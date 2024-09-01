/** * s_buffer.h Defines Data Struct for communication * Copyright © 2024 -
  Niels Neumann  <vatriani.nn@googlemail.com> * * This program is free software:
  you can redistribute it and/or modify * it under the terms of the GNU General
  Public License as published by * the Free Software Foundation, either version
  3 of the License, or * any later version. * * This program is distributed in
  the hope that it will be useful, * but WITHOUT ANY WARRANTY; without even the
  implied warranty of * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the * GNU General Public License for more details. * * You should have
  received a copy of the GNU General Public License * along with this program.
  If not, see <http://www.gnu.org/licenses/>. */


#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <bits/getopt_ext.h>

#include "include/s_buffer.h"



int serialPort;							// serial port handler
struct termios tty;					// struct for serial port settings
buffer buff;								// buffer struct

unsigned char isConnected;
unsigned char isDebug;
unsigned char isDevicePathChanged;

char* devicePath = "/dev/ttyUSB0"; // devault device



/**
 * helper function for opening and settings for serial port comunication
 */
void openSerial ( ) {
	serialPort = open ( devicePath, O_RDWR );

	if ( serialPort < 0 ) {
		printf ( "Error %i from open: %s\n", errno, strerror ( errno ) );
		exit( errno );
	}

	if ( tcgetattr ( serialPort, &tty ) != 0 ) {
		printf ( "Error %i from tcgetattr: %s\n", errno, strerror ( errno ) );
		exit( errno );
	}

	// some config changes
	cfsetispeed ( &tty, B9600 );
	cfsetospeed ( &tty, B9600 );

	// apply tty settings, also checking for error
	if ( tcsetattr ( serialPort, TCSANOW, &tty ) != 0 ) {
		printf ( "Error %i from tcsetattr: %s\n", errno, strerror ( errno ) );
		exit( errno );
	}

	isConnected = 1;
}



/**
 * helper function for closing serial port
 */
void closeSerial ( ) {
	close ( serialPort );
}



/**
 * output programname -h for help message
 */
inline void showHelp ( ) {
	printf ( "monitor for comunication with arduino sysmon\nuse: monitor\n\n" );
}



/**
 * output programname -v for version message
 */
inline void showVersion( ) {
	printf ( "monitor 0.1\nCopyright (C) 2024 Niels Neumann  <vatriani.nn@googlemail.com\n\
License GPLv3+: GNU GPL Version 3 or later <http://gnu.org/licenses/gpl.html>.\
\nThis is free software: you are free to change and redistribute it.\
\nThere is NO WARRANTY, to the extent permitted by law.\n\n" );
}



#ifdef DEBUG
/**
 * helper function to debug comunication
 */
inline void debFillBufferTestData ( ) {
	memcpy ( buff.cpuLines[0], "52", 3 );
	memcpy ( buff.cpuLines[1], "5200", 5 );
	memcpy ( buff.cpuLines[2], "31", 3 );

	memcpy ( buff.liquidLines[0], "1200", 5 );
	memcpy ( buff.liquidLines[1], "2100", 5 );
	memcpy ( buff.liquidLines[2], "31", 3 );

	memcpy ( buff.nvidiaLines[0], "60", 3 );
	memcpy ( buff.nvidiaLines[1], "5", 2 );
	memcpy ( buff.nvidiaLines[2], "15", 3 );

	memcpy ( buff.systemLines[0], "518", 2 );
	memcpy ( buff.systemLines[1], "0", 2 );
	memcpy ( buff.systemLines[2], "0", 2 );
}



/**
 * outputs buffer struct
 */
void debOutputBuffer ( ) {
	register unsigned int counter = 0;

	printf ( "cpu related\n" );
	for (counter = 0; counter < 3; counter++ ) {
		printf( ( const char* ) buff.cpuLines[counter] );
		printf("\n");
	}

	printf ( "watercool related\n" );
	for (counter = 0; counter < 3; counter++ ) {
		printf( ( const char* ) buff.liquidLines[counter] );
		printf("\n");
	}

	printf ( "gpu related\n" );
	for (counter = 0; counter < 3; counter++ ) {
		printf( ( const char* ) buff.nvidiaLines[counter] );
		printf("\n");
	}

	printf ( "system related\n" );
	for (counter = 0; counter < 3; counter++ ) {
		printf( ( const char* ) buff.systemLines[counter] );
		printf("\n");
	}
}
#endif



void init_monitor ( ) {
	isConnected = 0;
	register unsigned short int counter = 0;
	size_t allocSize = 5 * sizeof ( char8_t );

	// creating buffersize for numbers
	for (counter = 0; counter < 3; counter++ ) {
		buff.cpuLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.liquidLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.nvidiaLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.systemLines[ counter ] = ( char8_t* ) malloc ( allocSize );
	}

	openSerial ( );

#ifdef DEBUG
	debFillBufferTestData ( );
#endif
}



void close_monitor ( ) {
	register unsigned short int counter = 0;

	if ( serialPort != 0 )
		closeSerial ( );

	// free all allocated buffer
	for (counter = 0; counter < 3; counter++ ) {
		free ( buff.cpuLines[ counter ] );
		free ( buff.liquidLines[ counter ] );
		free ( buff.nvidiaLines[ counter ] );
		free ( buff.systemLines[ counter ] );
	}

	if ( isDevicePathChanged != 0 )
		free ( devicePath );
}



void loop_monitor ( ) {
#ifdef DEBUG
	if ( isDebug )
		debOutputBuffer ( );
#endif
}



int main (int argc, char** argv) {
	int opt;
	static struct option longOptions[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'v' },
		{ "debug", no_argument, 0, 'D' },
		{ "port", required_argument, 0, 'p'},
		{ 0, 0, 0, 0 },
	};
	int optionIndex;

	serialPort = 0;
	optionIndex = 0;
	isDevicePathChanged = 0;
	atexit(close_monitor); // exit handler for cleanup

	// opt management
	while (1) {
		opt = getopt_long ( argc, argv, "hvDp:", longOptions, &optionIndex );

		if ( opt == -1 ) break;

		switch ( opt ) {
			case 'h':
				showHelp ( );
				exit ( EXIT_SUCCESS );
			case 'v':
				showVersion ( );
				exit ( EXIT_SUCCESS );
			case 'D':
				isDebug = 1;
				break;
			case 'p':
				devicePath = malloc ( sizeof ( optarg ) * strlen ( optarg ) );
				memcpy ( devicePath, optarg, strlen ( optarg ) );
				isDevicePathChanged = 1
				break;
		}
	}

	init_monitor ( );

	loop_monitor ( );

	exit ( EXIT_SUCCESS );
}

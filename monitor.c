 /**
  *  \file       monitor.c
  *  \brief      Implements the watchdog for accuiring and sending data.
  *  \author     Niels Neumann
  *  \version    0.2
  *  \date       2024
  *  \copyright  GNU Public License v3
  *  \pre        First initialize the system.
  *  \bug
  *  \warning		 Early development
	*  \deprecated
  *  \todo
  */

#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <bits/getopt_ext.h>
#include <signal.h>

#include "include/s_buffer.h"



/// serial port handler
int serialPort;
/// struct for serial port settings
struct termios tty;
/// buffer struct
buffer buff;
/// holds value if connection is established
unsigned char isConnected;
/// holds value for activate debugging
unsigned char isDebug;
/// holds value for changed device
unsigned char isDevicePathChanged;
/// devault device
char* devicePath = "/dev/ttyUSB0";



/**
 * Run a system command localy and the return is written in rstr.
 * Used for single line Output.
 *
 * \param rstr pointer to return string
 * \param command runned command
 */
void getLineFromCommand ( char** rstr, char* command ) {
	FILE *fp;
  char retstr[1035];

	// Open the command for reading
  fp = popen ( command, "r" );
  if ( fp == NULL ) {
	  printf ( "Failed to run command\n" );
	  exit ( errno );
  }

  // Read the output
  fgets ( retstr, sizeof ( retstr ), fp );

	pclose(fp);

	(*rstr) = malloc ( strlen ( retstr ) );
	memcpy ( (*rstr), retstr, strlen (retstr) );
}


/**
 * Run a system command localy and the return is written in rstr.
 * Used for multiple line Output. Lines are divided by comma.
 *
 * \param rstr pointer to return string
 * \param command runned command
 */
void getLinesFromCommand ( char** rstr, char* command ) {
  FILE *fp;
  char retstr[1035];
	char strbuff[6000];

  // Open the command for reading
  fp = popen ( command, "r" );
  if ( fp == NULL ) {
    printf ("Failed to run command\n" );
    exit ( 1 );
  }

	strcpy ( strbuff, "" );

  // Read the output a line at a time - append it to strbuff
  while ( fgets ( retstr, sizeof ( retstr ), fp ) != NULL ) {
	  strcat( strbuff, retstr );
  }

  // close
  pclose(fp);

	(*rstr) = malloc ( strlen ( strbuff ) );
	memcpy ( (*rstr), strbuff, strlen (strbuff) );
}


/**
 * Reads and calculates CPU values.
 */
void getCpuValues ( ) {
	char* commandRet;

	getLineFromCommand (
		&commandRet,
		"cat /proc/cpuinfo | grep \"cpu MHz\" | head -n1 | cut -d\" \" -f3 | cut -d\".\" -f1");

	memcpy ( (void*) buff.cpuLines[1], commandRet, strlen ( commandRet ) - 1 );

	getLineFromCommand (
		&commandRet,
		"sensors | grep Tctl | cut -d\"+\" -f2" );

	memcpy ( (void*) buff.cpuLines[0], commandRet, strlen ( commandRet ) -6  );

	getLineFromCommand (
		&commandRet,
		"top -n1 | head -n3 | tail -n1 | cut -d\" \" -f11" );

	float tmp = atof ( commandRet );
		gcvt( 100.0 - tmp ,4 , (char*) buff.cpuLines[2]);
}


/**
 * Reads and calculates GPU values.
 */
void getGpuValues ( ) {
	char* commandRet;
	char* myPtr;
	int counter;

  getLineFromCommand (
		&commandRet,
		"/usr/bin/nvidia-smi | head -n10 | tail -n1");

	// divide string
	myPtr = strtok( commandRet, " ");
	counter = 0;

	while ( myPtr != NULL ) {
		switch ( counter ) {
			case 12: // utilization
				memcpy ( (void*) buff.nvidiaLines[1], myPtr, strlen ( myPtr ) - 1 );
				break;
			case 2: // get temp
				memcpy ( (void*) buff.nvidiaLines[0], myPtr, strlen ( myPtr ) - 1 );
				break;
			case 4: // get watt
				memcpy ( (void*) buff.nvidiaLines[2], myPtr, strlen ( myPtr ) - 1 );
				break;
		}
		// next field
  	myPtr = strtok(NULL, " ");
		counter++;
	}

	free ( commandRet );
}


/**
 * Reads and calculates liquid values.
 */
void getLiquidctlValues ( ) {
	char* commandRet;
	char* myPtr;
	int counter;

	getLinesFromCommand (
		&commandRet,
		"/usr/bin/liquidctl status | tail -n6 | head -n5");

	// divide string
	myPtr = strtok( commandRet, " ");
	counter = 0;

	while ( myPtr != NULL ) {
		switch ( counter ) {
			case 11: // utilization
				memcpy ( (void*) buff.liquidLines[1], myPtr, strlen ( myPtr ) + 1 );
				break;
			case 3: // get temp
				memcpy ( (void*) buff.liquidLines[2], myPtr, strlen ( myPtr ) + 1 );
				break;
			case 19: // get temp
				memcpy ( (void*) buff.liquidLines[0], myPtr, strlen ( myPtr ) + 1 );
				break;
		}
		// next field
  	myPtr = strtok(NULL, " ");
		counter++;
	}
}


/**
 * Reads and calculates system values.
 */
void getSystemValues ( ) {
	char* commandRet;
	char* myPtr;
	int counter;

	getLinesFromCommand (
		&commandRet,
		"sensors | grep \"System Fan\"");

	// divide string
	myPtr = strtok( commandRet, " ");
	counter = 0;

	while ( myPtr != NULL ) {
		switch ( counter ) {
			case 15: // utilization
				memcpy ( (void*) buff.systemLines[0], myPtr, strlen ( myPtr ) + 1 );
				break;
			case 27: // utilization
				memcpy ( (void*) buff.systemLines[1], myPtr, strlen ( myPtr ) + 1 );
				break;
			case 39: // utilization
				memcpy ( (void*) buff.systemLines[2], myPtr, strlen ( myPtr ) + 1 );
				break;
		}
		// next field
		myPtr = strtok(NULL, " ");
		counter++;
	}
}


/**
 * Helperfunction to fetch all values. Only for better reading.
 */
inline void getValues ( ) {
	getCpuValues ( );
	getGpuValues ( );
	getLiquidctlValues ( );
	getSystemValues ( );
}


/**
 * helper function for opening and settings for serial port comunication
 */
void openSerial ( ) {
	serialPort = open ( devicePath, O_RDWR );

	if ( serialPort < 0 ) {
		if ( errno == 2 )
			printf ( "No such device: %s\n", devicePath );
		else
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
 * \brief helper function for closing serial port
 */
inline void closeSerial ( ) {
	close ( serialPort );
}


/**
 * \brief Generates char buffer whitch contains the parsed struct buffer and sending it
 * over serial to the device.
 * \todo
 *   - simplify string copying with secound iterator for struct buffer.
 *   - move string handling to parseToBuffer(char**) function.
 */
void writeSerial ( ) {
	 char* buffer;
	 register char* iterator;
	 register unsigned int counter;

	 buffer = malloc ( sizeof ( char8_t ) * 9 * protoStrLength + sizeof ( char8_t ) * 2 );
	 iterator = buffer;

	 memcpy ( iterator, ( void* ) &protoVersion , 1 );
	 iterator += sizeof ( char8_t );

	 for ( counter = 0; counter < 3; counter++ ) {
		 memcpy (
			 iterator,
			 ( const char* ) buff.cpuLines[counter],
			 sizeof ( char8_t ) * strlen ( ( const char* ) buff.cpuLines[counter] ) );
		 iterator += sizeof ( char8_t ) * strlen ( ( const char* ) buff.cpuLines[counter] );
		 memcpy (
			 iterator,
			 ( void* ) &protoSeperator,
			 1 );
		 iterator += sizeof ( char8_t );
	 }

	 for ( counter = 0; counter < 3; counter++ ) {
		 memcpy (
			 iterator,
			 ( const char* ) buff.liquidLines[counter],
			 sizeof ( char8_t ) * strlen ( ( const char* ) buff.liquidLines[counter] ) );
		 iterator += sizeof ( char8_t ) * strlen ( ( const char* ) buff.liquidLines[counter] );
		 memcpy (
			 iterator,
			 ( void* ) &protoSeperator,
			 1 );
		 iterator += sizeof ( char8_t );
	 }

	 for ( counter = 0; counter < 3; counter++ ) {
		 memcpy (
			 iterator,
			 ( const char* ) buff.nvidiaLines[counter],
			 sizeof ( char8_t ) * strlen ( ( const char* ) buff.nvidiaLines[counter] ) );
		 iterator += sizeof ( char8_t ) * strlen ( ( const char* ) buff.nvidiaLines[counter] );
		 memcpy (
			 iterator,
			 ( void* ) &protoSeperator,
			 1 );
		 iterator += sizeof ( char8_t );
	 }

	 for ( counter = 0; counter < 3; counter++ ) {
		 memcpy (
			 iterator,
			 ( const char* ) buff.systemLines[counter],
			 sizeof ( char8_t ) * strlen ( ( const char* ) buff.systemLines[counter] ) );
		 iterator += sizeof ( char8_t ) * strlen ( ( const char* ) buff.systemLines[counter] );
		 memcpy (
			 iterator,
			 ( void* ) &protoSeperator,
			 1 );
		 iterator += sizeof ( char8_t );
	 }

	 iterator -= sizeof ( char8_t );
	 memcpy ( iterator, ( void* ) &protoLastByte, 1);

#ifdef DEBUG
	 if ( isDebug )
	 	 printf ( "%s\n", buffer );
#endif

	 write ( serialPort, buffer, sizeof ( buffer ) );

	 free ( buffer );
}


/**
 * Output monitor -h for help message
 */
inline void showHelp ( ) {
	printf ( "monitor for comunication with arduino sysmon\nuse: monitor\n\n" );
}


/**
 * Output monitor -v for version message
 */
inline void showVersion ( ) {
	printf ( "monitor 0.1\nCopyright (C) 2024 Niels Neumann  <vatriani.nn@googlemail.com\n\
License GPLv3+: GNU GPL Version 3 or later <http://gnu.org/licenses/gpl.html>.\
\nThis is free software: you are free to change and redistribute it.\
\nThere is NO WARRANTY, to the extent permitted by law.\n\n" );
}


#ifdef DEBUG
/**
 * DEBUG helper function to debug comunication
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
 * DEBUG outputs buffer struct
 */
void debOutputBuffer ( ) {
	printf("GPU Temp: %s\n    utilization: %s\n    Watt: %s\n",
		buff.nvidiaLines[0],
		buff.nvidiaLines[1],
		buff.nvidiaLines[2]);
	printf("CPU Temp: %s\n    takt: %s\n    utilization: %s\n",
	  buff.cpuLines[0],
		buff.cpuLines[1],
		buff.cpuLines[2]);
  printf("Liq fanspeed: %s\n    pumpspeed: %s\n    watertemp: %s\n",
		buff.liquidLines[0],
	  buff.liquidLines[1],
		buff.liquidLines[2]);
	printf("SYS fan1: %s\n    fan2: %s\n    fan3: %s\n",
		buff.systemLines[0],
		buff.systemLines[1],
		buff.systemLines[2]);
}
#endif


/**
 * Reciever for arduino init() function.
 */
void init_monitor ( ) {
	isConnected = 0;
	register unsigned short int counter = 0;
	size_t allocSize = protoStrLength * sizeof ( char8_t );

	// creating buffersize for numbers
	for (counter = 0; counter < 3; counter++ ) {
		buff.cpuLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.liquidLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.nvidiaLines[ counter ] = ( char8_t* ) malloc ( allocSize );
		buff.systemLines[ counter ] = ( char8_t* ) malloc ( allocSize );
	}

	openSerial ( );
}


/*
 * Helper function for cleanup memory and close the serial port.
 * Get automatically called everytime a exit() is used.
 */
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


/**
 * Like arduino loop() function. whitch contains the main loop of
 * recieving data from lm_sensors, parse data and sending it over serial.
 */
void loop_monitor ( ) {
	while ( 1 ) {
		getValues ( );
#ifdef DEBUG
	if ( isDebug )
		debOutputBuffer ( );
#endif
		writeSerial ( );
		sleep ( 1 );
	}
}


/**
 * Overrides signal handling for strg+c.
 */
void sig_handler(int signo) {
  if (signo == SIGINT) {
    printf("received SIGINT, closing watchdog\n");
		exit ( EXIT_SUCCESS );
	}
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

	// exit handler for cleanup
	atexit ( close_monitor );
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	printf("\ncan't catch SIGINT\n");

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
				isDevicePathChanged = 1;
				break;
		}
	}

	init_monitor ( );

	loop_monitor ( );

	exit ( EXIT_SUCCESS );
}

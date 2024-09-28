 /**
  *  \file       monitor.c
  *  \brief      Implements the watchdog for accuiring and sending data.
  *  \author     Niels Neumann
	*  \version    0.3
  *  \date       2024
  *  \copyright  GNU Public License v3
  *  \pre        First initialize the system.
  *  \bug
  *  \warning		 Early development
	*  \deprecated
	*  \todo			 CPU value for utilization
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
#include <libintl.h>
#include <time.h>

#include "config.h"
#include "include/s_buffer.h"
#include "include/defines.h"



/// serial port handler
int serialPort;
/// buffer struct
buffer buff;
/// holds value if connection is established
unsigned char isConnected;
/// holds value for activate debugging
unsigned char isDebug;
/// holds value for changed device
unsigned char isDevicePathChanged;
/// default device
char* devicePath = "/dev/ttyUSB0";
/// valid commands eg. whitelist for used systemprogramms
const char* execProgramms[9] = {
	"top",
	"cat",
	"sensors",
	"nvidia-smi",
	"liquidctl",
	"grep",
	"cut",
	"tail",
	"head"
};
static unsigned long uTimeStart;
static unsigned long timeStart;

float cpuavg(void) {
	unsigned long long user, nice, system, idle;
	unsigned long timeEnd, uTimeEnd;
	double ret = 0.0;

	FILE *data;
	data = fopen ( "/proc/stat", "r" );
	fscanf ( data, "%*s %llu %llu %llu %llu", &user, &nice, &system, &idle );
	fclose ( data );

	uTimeEnd = user + nice + system;
	timeEnd = user + nice + system + idle;

	if ( timeStart )
		ret = ( (double)( uTimeEnd - uTimeStart ) ) /
		( (double)( timeEnd - timeStart ) )*100.0;

	uTimeStart = uTimeEnd;
	timeStart = timeEnd;
	return ret;
}

/**
 * \brief Checks if valid commands is set. Block unwanted commands.
 */
unsigned int checkForValidCommand ( char* command ) {
	char testCommand[strlen ( command ) ];
	char* iterator;
	register unsigned int validCommandCounter;
	unsigned int countCommands;
	unsigned int ret;

	strcpy (testCommand, command);
	ret = 0;
	countCommands = 0;
	iterator = strtok ( testCommand, "|" );

	while ( iterator != NULL ) {
		validCommandCounter = 0;
		++countCommands;

		// remove whitespace in front of command
		iterator += strspn ( iterator , " " );

		// iterate over all valid commands
		for ( ; validCommandCounter < 9; validCommandCounter++ ) {
			if ( strncmp ( iterator, execProgramms[validCommandCounter],
				   strlen (execProgramms[validCommandCounter]) ) == 0 )
				ret += 1;
		}

		iterator = strtok ( NULL, "|" );
	}

#ifdef DEBUG
	if ( isDebug )
		printf ( _( "counted commands %i, counted valid commands %i\n" ), countCommands, ret );
#endif

	if ( ret == countCommands ) return 0;
	else return 1;
}



/**
 * \brief Run a system command localy and the return is written in rstr.
 * Used for single line Output.
 *
 * \param rstr pointer to return string
 * \param command runned command
 */
void getLineFromCommand ( char** rstr, char* command ) {
	FILE *process;
  char retstr[1035];

	if ( checkForValidCommand ( command ) ) {
		printf ( _("No valid command or isn't listed in whitelist.\n" ) );
		exit ( EXIT_FAILURE );
	}

	// Open the command for reading
	process = popen ( command, "r" );
	if ( process == NULL ) {
		printf ( _( "Failed to run command\n" ) );
	  exit ( errno );
  }

  // Read the output
	fgets ( retstr, sizeof ( retstr ), process );

	pclose ( process );

	(*rstr) = malloc ( strlen ( retstr ) );
	memcpy ( (*rstr), retstr, strlen (retstr) );
}



/**
 * \brief Run a system command localy and the return is written in rstr.
 * Used for multiple line Output. Lines are divided by comma.
 *
 * \param rstr pointer to return string
 * \param command runned command
 */
void getLinesFromCommand ( char** rstr, char* command ) {
	FILE *process;
  char retstr[1035];
	char strbuff[6000];

	if ( checkForValidCommand ( command ) ) {
		printf ( _( "No valid command or isn't listed in whitelist.\n" ) );
		exit ( EXIT_FAILURE );
	}

  // Open the command for reading
	process = popen ( command, "r" );
	if ( process == NULL ) {
		printf ( _( "Failed to run command\n" ) );
    exit ( 1 );
  }

	strcpy ( strbuff, "" );

  // Read the output a line at a time - append it to strbuff
	while ( fgets ( retstr, sizeof ( retstr ), process ) != NULL ) {
	  strcat( strbuff, retstr );
  }

  // close
	pclose ( process );

	(*rstr) = malloc ( strlen ( strbuff ) );
	memcpy ( (*rstr), strbuff, strlen (strbuff) );
}



/**
 * \brief Reads and calculates CPU values.
 */
void getCpuValues ( ) {
	char* commandRet;

	getLineFromCommand (
		&commandRet,
		"cat /proc/cpuinfo | grep \"cpu MHz\" | head -n1 | cut -d\" \" -f3 | cut -d\".\" -f1");
	memcpy ( (void*) buff.cpuLines[1], commandRet, strlen ( commandRet ) - 1 );

	getLineFromCommand (
		&commandRet,
		"sensors -u | grep Tctl -A1 | tail -n1 | cut -d\" \" -f4 | cut -b1-4");
	memcpy ( (void*) buff.cpuLines[0], commandRet, 4 );

	snprintf ( (char*)buff.cpuLines[2], protoStrLength - 1, "%'.1f", cpuavg ( ) );
	for ( char counter = 0; counter < protoStrLength; counter++ )
		if(buff.cpuLines[2][counter] == ',' ) buff.cpuLines[2][counter] = '.';
	free ( commandRet );
}



/**
 * \brief Reads and calculates GPU values.
 */
void getGpuValues ( ) {
	char* commandRet;
	char* iterator;
	int counter;

  getLineFromCommand (
		&commandRet,
		"nvidia-smi | head -n10 | tail -n1");

	// divide string
	iterator = strtok( commandRet, " ");
	counter = 0;

	while ( iterator != NULL ) {
		switch ( counter ) {
			case 12: // utilization
				memcpy ( (void*) buff.nvidiaLines[1], iterator, strlen ( iterator ) - 1 );
				break;
			case 2: // get temp
				memcpy ( (void*) buff.nvidiaLines[0], iterator, strlen ( iterator ) - 1 );
				break;
			case 4: // get watt
				memcpy ( (void*) buff.nvidiaLines[2], iterator, strlen ( iterator ) - 1 );
				break;
		}
		// next field
		iterator = strtok(NULL, " ");
		++counter;
	}

	free ( commandRet );
}



/**
 * \brief Reads and calculates liquid values.
 */
void getLiquidctlValues ( ) {
	char* commandRet;
	char* iterator;
	int counter;

	getLinesFromCommand (
		&commandRet,
		"liquidctl status | tail -n6 | head -n5");

	// divide string
	iterator = strtok( commandRet, " ");
	counter = 0;

	while ( iterator != NULL ) {
		switch ( counter ) {
			case 11: // utilization
				memcpy ( (void*) buff.liquidLines[1], iterator, strlen ( iterator ) + 1 );
				break;
			case 3: // get temp
				memcpy ( (void*) buff.liquidLines[2], iterator, strlen ( iterator ) + 1 );
				break;
			case 19: // get temp
				memcpy ( (void*) buff.liquidLines[0], iterator, strlen ( iterator ) + 1 );
				break;
		}
		// next field
		iterator = strtok(NULL, " ");
		++counter;
	}

	free ( commandRet );
}



/**
 * \brief Reads and calculates system values.
 */
void getSystemValues ( ) {
	char* commandRet;
	char* iterator;
	int counter;

	getLinesFromCommand (
		&commandRet,
		"sensors | grep \"System Fan\"");

	// divide string
	iterator = strtok( commandRet, " ");
	counter = 0;

	while ( iterator != NULL ) {
		switch ( counter ) {
			case 15: // utilization
				memcpy ( (void*) buff.systemLines[0], iterator, strlen ( iterator ) + 1 );
				break;
			case 27: // utilization
				memcpy ( (void*) buff.systemLines[1], iterator, strlen ( iterator ) + 1 );
				break;
			case 39: // utilization
				memcpy ( (void*) buff.systemLines[2], iterator, strlen ( iterator ) + 1 );
				break;
		}
		// next field
		iterator = strtok(NULL, " ");
		++counter;
	}

	free ( commandRet );
}



/**
 * \brief Helperfunction to fetch all values. Only for better reading.
 */
inline void getValues ( ) {
	getCpuValues ( );
	getGpuValues ( );
	getLiquidctlValues ( );
	getSystemValues ( );
}



/**
 * \brief Helper function for opening and settings for serial port comunication
 */
void openSerial ( ) {
	/// struct for serial port settings
	struct termios tty;


	serialPort = open ( devicePath, O_RDWR );

	if ( serialPort < 0 ) {
		if ( errno == 2 )
			printf ( _( "No such device: %s\n" ), devicePath );
		else
			printf ( _( "Error %i from open: %s\n" ), errno, strerror ( errno ) );
		exit ( errno );
	}

	if ( tcgetattr ( serialPort, &tty ) != 0 ) {
		printf ( _( "Error %i from tcgetattr: %s\n" ), errno, strerror ( errno ) );
		exit ( errno );
	}

	// some config changes
	cfsetispeed ( &tty, B9600 );
	cfsetospeed ( &tty, B9600 );

	// apply tty settings, also checking for error
	if ( tcsetattr ( serialPort, TCSANOW, &tty ) != 0 ) {
		printf ( _( "Error %i from tcsetattr: %s\n" ), errno, strerror ( errno ) );
		exit ( errno );
	}

	isConnected = 1;
}



/**
 * \brief Helper function for closing serial port
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

	 buffer = malloc ( sizeof ( char8_t ) * 12 * protoStrLength + sizeof ( char8_t ) * 2 );
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
 * \brief Output monitor -h for help message
 */
void showHelp ( ) {
	printf ( _( "monitor for comunication with arduino sysmon\nuse: monitor\n\n" ) );
}



/**
 * \brief Output monitor -v for version message
 */
void showVersion ( ) {
	printf ( _( "monitor 0.1\nCopyright (C) 2024 Niels Neumann  <vatriani.nn@googlemail.com\n\
License GPLv3+: GNU GPL Version 3 or later <http://gnu.org/licenses/gpl.html>.\
\nThis is free software: you are free to change and redistribute it.\
\nThere is NO WARRANTY, to the extent permitted by law.\n\n" ) );
}



#ifdef DEBUG
/**
 * \brief DEBUG helper function to debug comunication
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
 * \brief DEBUG outputs buffer struct
 */
void debOutputBuffer ( ) {
	printf ( _( "GPU Temp: %s\n    utilization: %s\n    Watt: %s\n" ),
		buff.nvidiaLines[0],
		buff.nvidiaLines[1],
		buff.nvidiaLines[2] );
	printf ( _( "CPU Temp: %s\n    takt: %s\n    utilization: %s\n" ),
	  buff.cpuLines[0],
		buff.cpuLines[1],
		buff.cpuLines[2] );
	printf ( _( "Liq fanspeed: %s\n    pumpspeed: %s\n    watertemp: %s\n" ),
		buff.liquidLines[0],
	  buff.liquidLines[1],
		buff.liquidLines[2] );
	printf ( _( "SYS fan1: %s\n    fan2: %s\n    fan3: %s\n" ),
		buff.systemLines[0],
		buff.systemLines[1],
		buff.systemLines[2] );
}
#endif



/**
 * \brief Reciever like arduino init() function.
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
 * \brief Helper function for cleanup memory and close the serial port.
 * Get automatically called everytime a exit() is used.
 */
void close_monitor ( ) {
	register unsigned short int counter;

	counter = 0;

	if ( serialPort != 0 )
		closeSerial ( );

	// free all allocated buffer
	for ( ; counter < 3; counter++ ) {
		free ( buff.cpuLines[ counter ] );
		free ( buff.liquidLines[ counter ] );
		free ( buff.nvidiaLines[ counter ] );
		free ( buff.systemLines[ counter ] );
	}

	if ( isDevicePathChanged != 0 )
		free ( devicePath );
}



/**
 * \brief Like arduino loop() function. whitch contains the main loop of
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
 * \brief Overrides signal handling for strg+c.
 */
void sig_handler ( int signo ) {
  if ( signo == SIGINT ) {
		printf ( _( "received SIGINT, closing watchdog\n" ) );
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
	timeStart = 0;

	// exit handler for cleanup
	atexit ( close_monitor );
	if ( signal ( SIGINT, sig_handler ) == SIG_ERR ) {
		printf ( _( "can't catch SIGINT\n" ) );
		exit ( EXIT_FAILURE );
	}

	setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

	// opt management
	while ( 1 ) {
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

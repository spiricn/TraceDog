/**
 * @file
 * @author Nikola Spiric <nikola.spiric.ns@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

/************************************
************* Includes **************
*************************************/
#include "td/td.h"

#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#if defined(WIN32)
	#include <Windows.h>
#endif

#if defined(__linux__)
	#include <pthread.h>
#endif

/************************************
************* Defines ***************
*************************************/

#ifdef TD_UNICODE
	#define td_printf wprintf
	#define td_fprintf fwprintf
	#define td_vsprintf vswprintf
	#define td_strtok wcstok
	#define td_strcpy wcscpy
	#define TD_STR_FMT L"%ls"
	#define TD_CHR_FMT L"%lc"
#else
	#define td_printf printf
	#define td_fprintf fprintf
	#define td_vsprintf vsnprintf
	#define td_strtok strtok 
	#define td_strcpy strcpy
	#define TD_STR_FMT "%s"
	#define TD_CHR_FMT "%c"

#endif

#define TD_MAX_TRACE_OUTPUTS ( 64 )

#define LOG_BUFFER_SIZE ( 1024 )

#if defined(WIN32)
#define	WIN32_COLOR_RED		( 0x04 )
#define	WIN32_COLOR_GREEN		( 0x02 )
#define	WIN32_COLOR_BLUE		( 0x01 )
#define	WIN32_COLOR_CYAN		( WIN32_COLOR_BLUE | WIN32_COLOR_GREEN )
#define	WIN32_COLOR_MAGENTA	( WIN32_COLOR_RED | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_YELLOW		( WIN32_COLOR_GREEN | WIN32_COLOR_RED )
#define	WIN32_COLOR_WHITE		( WIN32_COLOR_RED | WIN32_COLOR_GREEN | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_BLACK		( 0x00 )
#define WIN32_COLOR_INTENSE	( 0x08 )
#endif

#if defined(__linux__)

#define ANSI_CLR_RESET "\x1b[0m"

#define ANSI_CLR_FG_RED "31"
#define ANSI_CLR_FG_GREEN "32"
#define ANSI_CLR_FG_BLUE "34"
#define ANSI_CLR_FG_CYAN "36"
#define ANSI_CLR_FG_MAGENTA "35"
#define ANSI_CLR_FG_YELLOW "33"
#define ANSI_CLR_FG_WHITE "37"
#define ANSI_CLR_FG_BLACK "30"

#define ANSI_CLR_BG_RED "41"
#define ANSI_CLR_BG_GREEN "42"
#define ANSI_CLR_BG_BLUE "44"
#define ANSI_CLR_BG_CYAN "46"
#define ANSI_CLR_BG_MAGENTA "45"
#define ANSI_CLR_BG_YELLOW "43"
#define ANSI_CLR_BG_WHITE "47"
#define ANSI_CLR_BG_BLACK "40"


#endif

/************************************
************* Typedefs **************
*************************************/

typedef enum TdColorType_t {
	eTD_COLOR_BACKGROUND,
	eTD_COLOR_FOREGROUND,
} TdColorType; // </TdColorType>

typedef enum TdColor_t {
	eTD_COLOR_RED,
	eTD_COLOR_GREEN,
	eTD_COLOR_BLUE,
	eTD_COLOR_CYAN,
	eTD_COLOR_MAGENTA,
	eTD_COLOR_YELLOW,
	eTD_COLOR_WHITE,
	eTD_COLOR_BLACK,
} TdColor; // </TdColor>

/************************************
********* Global variables **********
*************************************/

typedef struct{
	TdTraceOutputId id;
	TdTraceLevel level;
	int enabled;
} TdTraceOutput;

static td_callbackFcn gCallbackFnc = NULL;

static FILE* gOutputFile = NULL;

static TdTraceOutput gTraceOutputs[3] = {
	{eTD_OUTPUT_CONSOLE,	eTD_LVL_VERBOSE, 1},
	{eTD_OUTPUT_FILE,		eTD_LVL_VERBOSE, 1},
	{eTD_OUTPUT_USER,		eTD_LVL_VERBOSE, 1}
};

#if defined(WIN32)
	CRITICAL_SECTION gCritSec;
	int gCritSecInitialized = 0;
#endif

#if defined(__linux__)
	pthread_mutex_t gMutex;
	int gMutexInitialized = 0;
#endif

/************************************
******* Function declarations *******
*************************************/

static void td_outputANSIColor(TdColor color, TdColorType type, int intense);

static TdError td_setConsoleColor(TdColor color, TdColorType type, int bold);

static TdError td_setColor(TdColor textColor, int textBold, TdColor backgroundColor, int backgroundBold);

static void td_getCurrentTime(const char* format, tdchar* result);

static void td_lock();

static void td_unlock();

/************************************
********* Function definitions ******
*************************************/

void td_getCurrentTime(const char* format, tdchar* result){
	time_t rawTime;
	struct tm* timeInfo;

	char bfr[128];

	time(&rawTime);
	timeInfo  = localtime(&rawTime);

	strftime(bfr, 128, format, timeInfo);

#ifdef TD_UNICODE
	mbstowcs(result, bfr, 128);
#else
	strcpy(result, bfr);
#endif
}

TdError td_setConsoleColor(TdColor color, TdColorType type, int bold){
#if defined(WIN32)	
	BOOL rc = FALSE;
	WORD attribs = 0;

	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;

	rc = GetConsoleScreenBufferInfo(stdoutHandle, &info);
	if(!rc){
		return eTD_ERROR;
	}

	// First niblet holds the information about foreground color (RGBI) while the second
	// one holds the information about background
	attribs = info.wAttributes;

	int colorCode = 0;

	if(color == eTD_COLOR_RED){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_GREEN){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_BLUE){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_CYAN){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_MAGENTA){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_YELLOW){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_WHITE){
		color = WIN32_COLOR_RED;
	}
	else if(color == eTD_COLOR_BLACK){
		color = WIN32_COLOR_RED;
	}

	if(bold){
		color |= WIN32_COLOR_INTENSE;

	}

	if(type == eTD_COLOR_BACKGROUND){
		attribs &= 0xFFFF00FF;
		attribs |= (WORD)((colorCode & 0xF) << 4);
	}
	else{
		attribs &= 0xFFFFFF00;
		attribs |= (WORD)(colorCode & 0xF );
	}

	rc = SetConsoleTextAttribute(stdoutHandle, attribs);

	return rc ? eTD_NO_ERROR : eTD_ERROR;
#elif defined(__linux__)
	td_outputANSIColor(color, type, bold);
	return eTD_NO_ERROR;
#else
	// TODO imlement this on unix
	#error Not implemented on this platform
#endif
}

void td_setOutputLevel(TdTraceOutputId id, TdTraceLevel level){
	gTraceOutputs[id].level = level;
}

void td_setOutputEnabled(TdTraceOutputId id, int enabled){
	gTraceOutputs[id].enabled = enabled;
}

TdError td_setColor(TdColor textColor, int textBold, TdColor backgroundColor, int backgroundBold){
	TdError rc = eTD_NO_ERROR;

	fprintf(stdout, ANSI_CLR_RESET);

	rc = td_setConsoleColor(textColor, eTD_COLOR_FOREGROUND, textBold);
	if(rc){
		return rc;
	}

	rc = td_setConsoleColor(backgroundColor, eTD_COLOR_BACKGROUND, backgroundBold);
	if(rc){
		return rc;
	}

	return rc;
}

void td_setCallbackFnc(td_callbackFcn fnc){
	gCallbackFnc = fnc;
}

void td_setFileOutput(FILE* output){
	gOutputFile = output;
}

TdError td_logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* fmt, ...){
	tdchar timeStamp[64];
	tdchar message[LOG_BUFFER_SIZE];
	tdchar* linePtr = NULL;

	va_list argList;

	// Color associated with the given trace level
	int levelColor = level == eTD_LVL_DEBUG || level == eTD_LVL_VERBOSE ? eTD_COLOR_WHITE : level == eTD_LVL_INFO ? eTD_COLOR_GREEN : level == eTD_LVL_WARNING ? eTD_COLOR_YELLOW : level == eTD_LVL_ERROR ? eTD_COLOR_RED : eTD_COLOR_WHITE;

	// Symbol representing the level (e.g. D for eTD_LVL_DEBUG)
	tdchar levelSymbol = level == eTD_LVL_DEBUG ? 'D' : level == eTD_LVL_VERBOSE ? 'V' : level == eTD_LVL_INFO ? 'I' : level == eTD_LVL_WARNING ? 'W' : level == eTD_LVL_ERROR ? 'E' : '?';

	tdchar levelStr[2] = {levelSymbol, 0};

	// Lock the mutex
	td_lock();

	// Get current time string
	td_getCurrentTime("%H:%M:%S", timeStamp);

	// Print the formated message into a buffer
	va_start(argList, fmt);

	td_vsprintf(message, LOG_BUFFER_SIZE, fmt, argList);

	va_end(argList);

	// Process the message line by line
	linePtr = td_strtok(message, TD_TEXT("\n"));

	while(linePtr != NULL){	
		// User output
		if(gTraceOutputs[eTD_OUTPUT_USER].enabled && gCallbackFnc && gTraceOutputs[eTD_OUTPUT_USER].level <= level){
			gCallbackFnc(tag, level, linePtr);
		}

		// Console output
		if(gTraceOutputs[eTD_OUTPUT_CONSOLE].enabled && gTraceOutputs[eTD_OUTPUT_CONSOLE].level <= level){
			// Timestamp
			td_setColor(eTD_COLOR_WHITE, 0, eTD_COLOR_BLACK, 0);
			td_printf(TD_TEXT("[") TD_STR_FMT TD_TEXT(" "), timeStamp);

			// Log level
			td_setColor(eTD_COLOR_WHITE, 1, levelColor, 0);
			td_printf(TD_TEXT(" ") TD_STR_FMT TD_TEXT(" "), levelStr);

			// Log tag
			td_setColor(eTD_COLOR_WHITE, 0, eTD_COLOR_BLACK, 0);
			td_printf(TD_TEXT(" ") TD_STR_FMT TD_TEXT("]: "), tag);

			// Message line
			td_setColor(levelColor,  level != eTD_LVL_VERBOSE ? 1: 0, eTD_COLOR_BLACK, 0);
			td_printf(TD_STR_FMT, linePtr);

			td_printf(TD_TEXT("\r\n"));

			// Restore console color to default
#if defined(WIN32)
			// TODO restore the actual saved state instead of white/black
			td_setColor(eTD_COLOR_WHITE, 0, eTD_COLOR_BLACK, 0);
#else
			fprintf(stdout, ANSI_CLR_RESET);
#endif
		}

		// File output
		if(gTraceOutputs[eTD_OUTPUT_FILE].enabled && gTraceOutputs[eTD_OUTPUT_CONSOLE].level <= level && gOutputFile){
			// Plain text output
			td_fprintf(gOutputFile, TD_TEXT("[%s ") TD_STR_FMT TD_TEXT(" ]: ") TD_STR_FMT TD_TEXT("\r\n"), timeStamp, levelStr, linePtr);

			// Flush the file output after every log
			fflush(gOutputFile);
		}

		// Next line
		linePtr = td_strtok(NULL, TD_TEXT("\n"));
	}

	td_unlock();

	return eTD_NO_ERROR;
}

void td_lock(){
#if defined(WIN32)
	if(!gCritSecInitialized){
		InitializeCriticalSection(&gCritSec);
		gCritSecInitialized = 1; 
	}

	EnterCriticalSection(&gCritSec);
#elif defined(__linux__)
	if(!gMutexInitialized){
		pthread_mutex_init(&gMutex, NULL);
		gMutexInitialized = 1;
	}

	pthread_mutex_lock(&gMutex);
#else
	// TODO
	#error Not implemented on this platform
#endif
}

void td_unlock(){
#if defined(WIN32)
	LeaveCriticalSection(&gCritSec);
#elif defined(__linux__)
	pthread_mutex_unlock(&gMutex);
#else
	// TODO
	#error Not implemented on this platform
#endif
}

static void td_outputANSIColor(TdColor color, TdColorType type, int intense){
	char code[64] = "\x1b[";

	if(color == eTD_COLOR_RED){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_RED : ANSI_CLR_BG_RED);
	}
	else if(color == eTD_COLOR_GREEN){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_GREEN : ANSI_CLR_BG_GREEN);
	}
	else if(color == eTD_COLOR_BLUE){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_BLUE : ANSI_CLR_BG_BLUE);
	}
	else if(color == eTD_COLOR_CYAN){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_CYAN : ANSI_CLR_BG_CYAN);
	}
	else if(color == eTD_COLOR_MAGENTA){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_MAGENTA : ANSI_CLR_BG_MAGENTA);
	}
	else if(color == eTD_COLOR_YELLOW){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_YELLOW : ANSI_CLR_BG_YELLOW);
	}
	else if(color == eTD_COLOR_WHITE){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_WHITE : ANSI_CLR_BG_WHITE);
	}
	else if(color == eTD_COLOR_BLACK){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_BLACK : ANSI_CLR_BG_BLACK);
	}

	if(intense){
		strcat(code, ";1m");
	}
	else{
		strcat(code, "m");
	}

	fprintf(stdout, "%s", code);
}

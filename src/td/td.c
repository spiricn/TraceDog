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


/************************************
********* Global variables **********
*************************************/

typedef struct{
	enum TdTraceOutputId id;
	enum TdTraceLevel level;
	int enabled;
} TdTraceOutput;

static td_callbackFcn gCallbackFnc = NULL;

static FILE* gOutputFile = NULL;

enum TdFileOutputType gFileOutputType = eTD_FILE_HTML;

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

static enum TdError td_setConsoleColor(int color, enum TdColorType type);

static enum TdError td_setColor(int textColor, int backgroundColor);

static void td_getCurrentTime(const char* format, tdchar* result);

static void td_lock();

static void td_unlock();

static void td_getHTMLColor(int color, char res[7]);

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

enum TdError td_setConsoleColor(int color, enum TdColorType type){
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
	// one holds the information about backgorund
	attribs = info.wAttributes;

	if(type == eTD_COLOR_BACKGROUND){
		attribs &= 0xFFFF00FF;
		attribs |= (WORD)((color & 0xF) << 4);
	}
	else{
		attribs &= 0xFFFFFF00;
		attribs |= (WORD)(color & 0xF );
	}

	rc = SetConsoleTextAttribute(stdoutHandle, attribs);

	return rc ? eTD_NO_ERROR : eTD_ERROR;
#elif defined(__linux__)
	// TODO
	return eTD_NO_ERROR;
#else
	// TODO imlement this on unix
	#error Not implemented on this platform
#endif
}

void td_setOutputLevel(enum TdTraceOutputId id, enum TdTraceLevel level){
	gTraceOutputs[id].level = level;
}

void td_setOutputEnabled(enum TdTraceOutputId id, int enabled){
	gTraceOutputs[id].enabled = enabled;
}

enum TdError td_setColor(int textColor, int backgroundColor){
	enum TdError rc = eTD_NO_ERROR;

	rc = td_setConsoleColor(textColor, eTD_COLOR_FOREGROUND);
	if(rc){
		return rc;
	}

	rc = td_setConsoleColor(backgroundColor, eTD_COLOR_BACKGROUND);
	if(rc){
		return rc;
	}

	return rc;
}

void td_setCallbackFnc(td_callbackFcn fnc){
	gCallbackFnc = fnc;
}

void td_setFileOutput(FILE* output){
	if(output == NULL && gOutputFile && gFileOutputType == eTD_FILE_HTML){
		// Write HTML trailer
		td_fprintf(gOutputFile, 
			TD_TEXT( "\n</table>\n</body>\n</html>\n"));
	}

	gOutputFile = output;
}

enum TdError td_logMessage(const tdchar* tag, enum TdTraceLevel level, const tdchar* fmt, ...){
	tdchar timeStamp[64];
	tdchar message[LOG_BUFFER_SIZE];
	tdchar* linePtr = NULL;

	va_list argList;

	// Color associated with the given trace level
	int levelColor = level == eTD_LVL_DEBUG || level == eTD_LVL_VERBOSE ? TD_COLOR_WHITE : level == eTD_LVL_INFO ? TD_COLOR_GREEN : level == eTD_LVL_WARNING ? TD_COLOR_YELLOW : level == eTD_LVL_ERROR ? TD_COLOR_RED : TD_COLOR_WHITE;

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
			td_printf(TD_TEXT("[") TD_STR_FMT TD_TEXT(" "), timeStamp);

			// Log level
			td_setColor(TD_COLOR_WHITE | TD_COLOR_INTENSE, levelColor);
			td_printf(TD_TEXT(" ") TD_STR_FMT TD_TEXT(" "), levelStr);

			// Log tag
			td_setColor(TD_COLOR_WHITE, TD_COLOR_BLACK);
			td_printf(TD_TEXT(" ") TD_STR_FMT TD_TEXT("]: "), tag);

			// Message line
			td_setColor(levelColor | (level != eTD_LVL_VERBOSE ? TD_COLOR_INTENSE : 0), TD_COLOR_BLACK);
			td_printf(TD_STR_FMT, linePtr);

			td_printf(TD_TEXT("\r\n"));

			// Restore console color to default
			// TODO restore the actual saved state instead of white/black
			td_setColor(TD_COLOR_WHITE, TD_COLOR_BLACK);
		}

		// File output
		if(gTraceOutputs[eTD_OUTPUT_FILE].enabled && gTraceOutputs[eTD_OUTPUT_CONSOLE].level <= level && gOutputFile){
			if(gFileOutputType == eTD_FILE_HTML){
				// HTML output
				tdchar htmlColor[7];
				htmlColor[6] = 0;
				td_getHTMLColor(levelColor, htmlColor);

				
				td_fprintf(gOutputFile,
					TD_TEXT("<tr>\n") // start of row
					TD_TEXT("<td>") TD_STR_FMT TD_TEXT("</td>\n") // timestamp
					TD_TEXT("<td> <font color=\"") TD_STR_FMT TD_TEXT("\">") TD_STR_FMT TD_TEXT("</font> </td>\n")// level
					TD_TEXT("<td>") TD_STR_FMT TD_TEXT("</td>\n") // tag
					TD_TEXT("<td>") TD_STR_FMT TD_TEXT("</td>\n") // message
					TD_TEXT("</tr>\n"), // end of row
					timeStamp, htmlColor, levelStr, tag, message);
			}
			else{
				// Plain text output
				td_fprintf(gOutputFile, TD_TEXT("[%s ") TD_STR_FMT TD_TEXT(" ]: ") TD_STR_FMT TD_TEXT("\r\n"), timeStamp, levelStr, linePtr);
			}

			// Flush the file output after every log either way
			fflush(gOutputFile);
		}

		// Next line
		linePtr = td_strtok(NULL, TEXT("\n"));
	}

	td_unlock();

	return eTD_NO_ERROR;
}

void td_setFileOutputType(enum TdFileOutputType type){
	gFileOutputType = type;
	if(gOutputFile && type == eTD_FILE_HTML){
		// TODO set file encoding for unicode, the browser is having some trouble reading
		// the current version of this when TD_UNICODE is set
		td_fprintf(gOutputFile, 
			TD_TEXT("<html>\n")
			TD_TEXT("<body text=\"white\">\n")
			TD_TEXT("<table border=1 bgcolor=\"#000000\">\n")
		);
	}
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


void td_getHTMLColor(int color, tdchar res[7]){
	// Red
	res[0] = color & TD_COLOR_RED ? 'F' : '0';
	res[1] = color & TD_COLOR_RED ? 'F' : '0';

	// Green
	res[2] = color & TD_COLOR_GREEN ? 'F' : '0';
	res[3] = color & TD_COLOR_GREEN ? 'F' : '0';

	// Blue
	res[4] = color & TD_COLOR_BLUE ? 'F' : '0';
	res[5] = color & TD_COLOR_BLUE ? 'F' : '0';
}

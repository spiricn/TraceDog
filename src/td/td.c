/************************************
************* Includes **************
*************************************/
#include "td/td.h"

#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#if defined(WIN32)
	#include <Windows.h>
#endif


/************************************
************* Defines ***************
*************************************/
#ifdef TD_UNICODE
	#define TD_TEXT(s) L ## s
	#define tdprintf wprintf
	#define tdfprintf fwprintf
	#define tdvsprintf vswprintf
	#define TD_STR_FMT L"%ls"
	#define TD_CHR_FMT L"%lc"
#else
	#define TD_TEXT(s) s
	#define tdprintf printf
	#define tdfprintf fprintf
	#define tdvsprintf vsprintf
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

static TdTraceOutput gTraceOutputs[3] = {
	{eTD_OUTPUT_CONSOLE,	eTD_LVL_VERBOSE, 1},
	{eTD_OUTPUT_FILE,		eTD_LVL_VERBOSE, 1},
	{eTD_OUTPUT_USER,		eTD_LVL_VERBOSE, 1}
};


/************************************
******* Function declarations *******
*************************************/

static enum TdError td_setConsoleColor(int color, enum TdColorType type);

static enum TdError td_setColor(int textColor, int backgroundColor);

static void td_getCurrentTime(const char* format, char* result);

/************************************
********* Function definitions ******
*************************************/

void td_getCurrentTime(const char* format, char* result){
	time_t rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo  = localtime(&rawTime);

	strftime(result, 128, format, timeInfo);
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

#else
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
	gOutputFile = output;
}

enum TdError td_logMessage(const tdchar* tag, enum TdTraceLevel level, const tdchar* fmt, ...){
	char timeStamp[64];
	tdchar message[LOG_BUFFER_SIZE];
	va_list argList;

	// Color associated with the given trace level
	int levelColor = level == eTD_LVL_DEBUG || level == eTD_LVL_VERBOSE ? TD_COLOR_WHITE : level == eTD_LVL_INFO ? TD_COLOR_GREEN : level == eTD_LVL_WARNING ? TD_COLOR_YELLOW : level == eTD_LVL_ERROR ? TD_COLOR_RED : TD_COLOR_WHITE;

	// Symbol representing the level (e.g. D for eTD_LVL_DEBUG)
	tdchar levelSymbol = level == eTD_LVL_DEBUG ? 'D' : level == eTD_LVL_VERBOSE ? 'V' : level == eTD_LVL_INFO ? 'I' : level == eTD_LVL_WARNING ? 'W' : level == eTD_LVL_ERROR ? 'E' : '?';

	tdchar levelStr[2] = {levelSymbol, '\0'};

	// Get current time string
	td_getCurrentTime("%H:%M:%S", timeStamp);

	// Print the formated message into a buffer
	va_start(argList, fmt);

	tdvsprintf(message, fmt, argList);

	va_end(argList);

	 
	// User outptu
	if(gTraceOutputs[eTD_OUTPUT_USER].enabled && gCallbackFnc && gTraceOutputs[eTD_OUTPUT_USER].level <= level){
		gCallbackFnc(tag, level, message);
	}

	// Console output
	if(gTraceOutputs[eTD_OUTPUT_CONSOLE].enabled && gTraceOutputs[eTD_OUTPUT_CONSOLE].level <= level){
		tdprintf(TD_TEXT("[%s "), timeStamp);

		td_setColor(TD_COLOR_WHITE | TD_COLOR_INTENSE, levelColor);
		tdprintf(TD_TEXT(" "));
		tdprintf(levelStr);
		tdprintf(TD_TEXT(" "));

		td_setColor(TD_COLOR_WHITE, TD_COLOR_BLACK);
		tdprintf(TD_TEXT(" "));

		tdprintf(tag);

		tdprintf(TD_TEXT("]: "));


		td_setColor(levelColor | (level != eTD_LVL_VERBOSE ? TD_COLOR_INTENSE : 0), TD_COLOR_BLACK);
		tdprintf(message);

		tdprintf(TD_TEXT("\r\n"));

		td_setColor(TD_COLOR_WHITE, TD_COLOR_BLACK);
	}

	// File output
	if(gTraceOutputs[eTD_OUTPUT_FILE].enabled && gTraceOutputs[eTD_OUTPUT_CONSOLE].level <= level && gOutputFile){
		tdfprintf(gOutputFile, TD_TEXT("[%s " TD_STR_FMT " ]: " TD_STR_FMT "\r\n"), timeStamp, levelStr, message);
		fflush(gOutputFile);
	}

	return eTD_NO_ERROR;
}
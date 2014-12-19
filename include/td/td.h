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

#ifndef TDTD_H
#define TDTD_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Helper macros used to create a wide versions of __FILE__ and __FUNCTION__
 */
#define TD_WIDEN2(x)  L ## x
#define TD_WIDEN(x) TD_WIDEN2(x)

#ifdef TD_UNICODE
	typedef wchar_t tdchar;
	#define TD_TEXT(s) L ## s
	#define TD_FILE TD_WIDEN(__FILE__)
	#define TD_FUNCTION TD_WIDEN(__FUNCTION__)
#else
	typedef char tdchar;
	#define TD_TEXT(s) s
	#define TD_FILE __FILE__
	#define TD_FUNCTION __FUNCTION__
#endif


enum TdError {
	eTD_NO_ERROR = 0x00,
	eTD_ERROR = 0x1
}; // </TdError>

enum TdTraceLevel{
	eTD_LVL_VERBOSE = 0,
	eTD_LVL_DEBUG = 1,
	eTD_LVL_INFO = 2,
	eTD_LVL_WARNING = 3,
	eTD_LVL_ERROR = 4,
}; // </TdTraceLevel>

enum TdColorType {
	eTD_COLOR_BACKGROUND,
	eTD_COLOR_FOREGROUND,
}; // </TdColorType>

enum TdTraceOutputId{
	eTD_OUTPUT_CONSOLE = 0x0,
	eTD_OUTPUT_FILE = 0x1,
	eTD_OUTPUT_USER = 0x2
}; // </TdTraceOutputId>

enum TdFileOutputType{
	eTD_FILE_PLAIN_TEXT,
	eTD_FILE_HTML
}; // </TdFileOutputType>

/**
 * Color bitfields. Entire color related information can be incoded in a single niblet.
 */
#define	TD_COLOR_RED		( 0x04 )
#define	TD_COLOR_GREEN		( 0x02 )
#define	TD_COLOR_BLUE		( 0x01 )
#define	TD_COLOR_CYAN		( TD_COLOR_BLUE | TD_COLOR_GREEN )
#define	TD_COLOR_MAGENTA	( TD_COLOR_RED | TD_COLOR_BLUE )
#define	TD_COLOR_YELLOW		( TD_COLOR_GREEN | TD_COLOR_RED )
#define	TD_COLOR_WHITE		( TD_COLOR_RED | TD_COLOR_GREEN | TD_COLOR_BLUE )
#define	TD_COLOR_BLACK		( 0x00 )
#define TD_COLOR_INTENSE	( 0x08 )

/** User callback function. */
typedef void (*td_callbackFcn)(const tdchar* tag, enum TdTraceLevel level, const tdchar* message);

/**
 * @brief Formats and sends the message to all enabled outputs (i.e. console, file and user).
 *
 * @param tag	Module tag string.
 * @param level Message trace level, used to filter messages via td_setOutputLevel.
 * @param fmt	Message format.
 * @param ...	Message format arguments.
 * @return - Returns eTD_NO_ERROR on success, eTD_ERROR otherwise.
 */
enum TdError td_logMessage(const tdchar* tag, enum TdTraceLevel level, const tdchar* message, ...);

/**
 * @brief Sets the user callback function.
 * User callback function is invoked by 'td_logMessage' calls if the level is appropriate
 * and user ouput is enabled via 'td_setOutputEnabled' with eTD_OUTPUT_USER as ID.
 *
 * @param fnc	Callback function pointer
 */
void td_setCallbackFnc(td_callbackFcn fnc);

/**
 * Sets file destination for eTD_OUTPUT_FILE trace output.
 *
 * If the file output type is HTML and 'output' param is NULL a HTML trailer shall be written.
 *
 * @param output	File output pointer, can be NULL which disables this trace output
 */
void td_setFileOutput(FILE* output);

/**
 * @brief Sets output level for the given output ID.
 * If a message trace level is greater than the output level the message will be ignored.
 *
 * @param id		Trace output id, can be one of the following:
 *						eTD_OUTPUT_CONSOLE - STDOUT console output. 
 *						eTD_OUTPUT_FILE    - File output.
 *						eTD_OUTPUT_USER    - User callback output
 * @param level		Actual trace output level.
 */
void td_setOutputLevel(enum TdTraceOutputId id, enum TdTraceLevel level);


/**
 * @brief Enables or disables a single trace output.
 *
 * @param id		Trace output ID
 * @param enabled	1 to enable output, 0 otherwise
 */
void td_setOutputEnabled(enum TdTraceOutputId id, int enabled);

/**
 * @brief Changes file output format.
 * If the output file is already set, and 'type' param is HTML a HTML header will be written
 *
 * @param type	Can be either plain text or formatted HTML.
 */
void td_setFileOutputType(enum TdFileOutputType type);


/**
 * Basic log helper macros removing the need to call 'td_logMessage' directly.
 */
#ifndef TD_DISABLED
	#define LOGV(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_VERBOSE,	fmt, ##__VA_ARGS__); }while(0)
	#define LOGD(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_DEBUG,	fmt, ##__VA_ARGS__); }while(0)
	#define LOGI(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_INFO,	fmt, ##__VA_ARGS__); }while(0)
	#define LOGW(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_WARNING,	fmt, ##__VA_ARGS__); }while(0)
	#define LOGE(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_ERROR,	fmt, ##__VA_ARGS__); }while(0)
#else
	#define LOGV(fmt, ...)
	#define LOGD(fmt, ...)
	#define LOGI(fmt, ...)
	#define LOGW(fmt, ...)
	#define LOGE(fmt, ...)
#endif

/**
 * Tracer macros, extended LOGx macros with file, function and line information.
 */
#define TD_TRACE_INFO  TD_TEXT("{%s - %s @ %d} ")

#define TRACEV(fmt, ...) LOGV(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACED(fmt, ...) LOGD(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEI(fmt, ...) LOGI(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEW(fmt, ...) LOGW(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEE(fmt, ...) LOGE(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)

/**
 * Assert related macros
 */
#define TD_ASSERT(val, fmt, ...) do{ if(!(val)){ TRACEE(TD_TEXT("Assertion failed: \"") fmt TD_TEXT("\""), ##__VA_ARGS__); abort();} }while(0)

#define TD_ASSERT_LT(val1, val2, fmt, ...) TD_ASSERT(val1 < val2, fmt, __VA_ARGS__)
#define TD_ASSERT_LTE(val1, val2, fmt, ...) TD_ASSERT(val1 <= val2, fmt, __VA_ARGS__)
#define TD_ASSERT_GT(val1, val2, fmt, ...) TD_ASSERT(val1 > val2, fmt, __VA_ARGS__)
#define TD_ASSERT_GTE(val1, val2, fmt, ...) TD_ASSERT(val1 >= val2, fmt, __VA_ARGS__)
#define TD_ASSERT_NE(val1, val2, fmt, ...) TD_ASSERT(val1 != val2, fmt, __VA_ARGS__)
#define TD_ASSERT_E(val1, val2, fmt, ...) TD_ASSERT(val1 == val2, fmt, __VA_ARGS__)

/** Default trace tag */
#ifndef TD_TRACE_TAG
	#define TD_TRACE_TAG ("")
#endif

#ifdef __cplusplus
} // </extern "C"{>
#endif

#endif // </TDTD_H>

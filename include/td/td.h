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

/*============================================================================
 *   INCLUDES
 *===========================================================================*/

#include <stdio.h>
#include <stdlib.h>

/*============================================================================
 *   DEFINES
 *===========================================================================*/

#ifdef __cplusplus
extern "C"{
#endif

#define TD_VERSION "1.0.0"

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

#ifndef DISABLE_TD_WRAPPER_MACROS

/**
 * Basic log helper macros removing the need to call 'td_logMessage' directly.
 */
#define LOGV(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_VERBOSE,	fmt, ##__VA_ARGS__); }while(0)
#define LOGD(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_DEBUG,	fmt, ##__VA_ARGS__); }while(0)
#define LOGI(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_INFO,	fmt, ##__VA_ARGS__); }while(0)
#define LOGW(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_WARNING,	fmt, ##__VA_ARGS__); }while(0)
#define LOGE(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_ERROR,	fmt, ##__VA_ARGS__); }while(0)

/**
 * Tracer macros, extended LOGx macros with file, function and line information.
 */
#define TD_TRACE_INFO  TD_TEXT("[ %s - %s @ %d ]: ")

#define TRACEV(fmt, ...) LOGV(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACED(fmt, ...) LOGD(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEI(fmt, ...) LOGI(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEW(fmt, ...) LOGW(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)
#define TRACEE(fmt, ...) LOGE(TD_TRACE_INFO fmt, TD_FILE, TD_FUNCTION, __LINE__, ## __VA_ARGS__)

/**
 * Assert related macros.
 */
#define TD_ASSERT(val, fmt, ...) do{ if(!(val)){ TRACEE(TD_TEXT("Assertion failed: \"") fmt TD_TEXT("\""), ##__VA_ARGS__); abort();} }while(0)
#define TD_ASSERT_LT(val1, val2, fmt, ...) TD_ASSERT((val1) < (val2), fmt, __VA_ARGS__)
#define TD_ASSERT_LTE(val1, val2, fmt, ...) TD_ASSERT((val1) <= (val2), fmt, __VA_ARGS__)
#define TD_ASSERT_GT(val1, val2, fmt, ...) TD_ASSERT((val1) > (val2), fmt, __VA_ARGS__)
#define TD_ASSERT_GTE(val1, val2, fmt, ...) TD_ASSERT((val1) >= (val2), fmt, __VA_ARGS__)
#define TD_ASSERT_NE(val1, val2, fmt, ...) TD_ASSERT((val1) != (val2), fmt, __VA_ARGS__)
#define TD_ASSERT_E(val1, val2, fmt, ...) TD_ASSERT((val1) == (val2), fmt, __VA_ARGS__)

/**
* Default trace tag
*/
#ifndef TD_TRACE_TAG
#define TD_TRACE_TAG ("")
#endif

#endif

/** Invalid output handle value */
#define TD_INVALID_OUTPUT NULL

/*============================================================================
 *   TYPE DEFINITIONS
 *===========================================================================*/

/** Library error codes. */
typedef enum TdError_t {
	eTD_NO_ERROR = 0x00,
	eTD_ERROR = 0x1
} TdError;

/** Message levels. */
typedef enum TdTraceLevel_t {
	eTD_LVL_VERBOSE,
	eTD_LVL_DEBUG,
	eTD_LVL_INFO,
	eTD_LVL_WARNING ,
	eTD_LVL_ERROR,
} TdTraceLevel;

/** File format */
typedef enum TdFileFormat_t{
	eTD_FMT_PLAIN_TEXT,
	eTD_FMT_HTML
} TdFileFormat;

/** User callback function. */
typedef void (*td_callbackFcn)(const tdchar* tag, TdTraceLevel level, const tdchar* message, void* userData);

/** Handle used to reference a single output */
typedef void* TdOutputHandle;

/*============================================================================
 *   FUNCTION DECLARATIONS
 *===========================================================================*/

/**
 * Logs a single message with the given tag and level.
 * 
 * @param tag Module tag string.
 * @param level Message level.
 * @param message Message string.
 *
 * @return eTD_NO_ERROR if successful, eTD_ERROR otherwise.
 */
TdError td_logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* message, ...);

/**
 * Creates a text file output.
 *
 * @param path File location.
 * @param fileFormat Determines the format of the written text (e.g. plain text or HTML)
 *
 * @return TD_INVALID_OUTPUT on error, otherwise a valid output handle.
 */
TdOutputHandle td_createFileOutput(const tdchar* path, TdFileFormat fileFormat);

/**
 * Creates a console (i.e. stdout) output.
 *
 * @return TD_INVALID_OUTPUT on error, otherwise a valid output handle.
 */
TdOutputHandle td_createConsoleOutput();

/**
 * Gets a handle to the default console output created on startup.
 *
 * @return A valid output handle.
 */
TdOutputHandle td_getDefaultOutput();

/**
 * Destroys an output with the given handle
 *
 * @param handle A valid output handle.
 *
 * @return eTD_NO_ERROR if successful, eTD_ERROR otherwise.
 */
TdError td_destroyOutput(TdOutputHandle handle);

/**
 * Sets a user callback function and opaque data. This function is called
 * every time a message is logged.
 *
 * @param fnc Function pointer. May be NULL.
 * @param userData User data included in the callback. May be NULL.
 *
 * @return eTD_NO_ERROR if successful, eTD_ERROR otherwise.
 */
TdError td_setCallbackFnc(td_callbackFcn fnc, void* userData);

/**
 * Sets a log level of the given output. If a logged message level is below the output's
 * the message will not be logged.
 *
 * @param handle A valid output handle.
 * @param level New output level.
 *
 * @return eTD_NO_ERROR if successful, eTD_ERROR otherwise.
 */
TdError td_setOutputLevel(TdOutputHandle handle, TdTraceLevel level);

#ifdef __cplusplus
} // </extern "C"{>
#endif

#endif /* TDTD_H */

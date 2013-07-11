#ifndef TDTD_H
#define TDTD_H

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

#ifdef TD_UNICODE
	typedef wchar_t tdchar;
#else
	typedef char tdchar;
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

#define	TD_COLOR_RED		( 0x04 )
#define	TD_COLOR_GREEN		( 0x02 )
#define	TD_COLOR_BLUE		( 0x01 )
#define	TD_COLOR_CYAN		( TD_COLOR_BLUE | TD_COLOR_GREEN )
#define	TD_COLOR_MAGENTA	( TD_COLOR_RED | TD_COLOR_BLUE )
#define	TD_COLOR_YELLOW		( TD_COLOR_GREEN | TD_COLOR_RED )
#define	TD_COLOR_WHITE		( TD_COLOR_RED | TD_COLOR_GREEN | TD_COLOR_BLUE )
#define	TD_COLOR_BLACK		( 0x00 )
#define TD_COLOR_INTENSE	( 0x08 )

typedef void (*td_callbackFcn)(const tdchar* tag, enum TdTraceLevel level, const tdchar* message);

/**
 * Formats and sends the message to all enabled outputs (i.e. console, file and user).
 *
 * @param tag - Module tag string.
 * @param level - Message trace level, used to filter messages via td_setOutputLevel.
 * @param fmt - Message format.
 * @param ... - Message format arguments.
 * @return - Returns eTD_NO_ERROR on success, eTD_ERROR otherwise.
 */
enum TdError td_logMessage(const tdchar* tag, enum TdTraceLevel level, const tdchar* message, ...);

/**
 * Sets the user callback function.
 * User callback function is invoked by 'td_logMessage' calls if the level is appropriate
 * and user ouput is enabled via 'td_setOutputEnabled' with eTD_OUTPUT_USER as ID.
 *
 * @param fnc - Callback function pointer
 */
void td_setCallbackFnc(td_callbackFcn fnc);

/**
 * Sets file destination for eTD_OUTPUT_FILE trace output.
 *
 * @param output - File output pointer, can be NULL which disables this trace output
 */
void td_setFileOutput(FILE* output);

/**
 * Sets output level for the given output ID.
 * If a message trace level is greater than the output level the message will be ignored.
 *
 * @param id - Trace output id, can be one of the following:
 *						eTD_OUTPUT_CONSOLE - STDOUT console output. 
 *						eTD_OUTPUT_FILE    - File output.
 *						eTD_OUTPUT_USER    - User callback output
 * @param level - Actual trace output level.
 */
void td_setOutputLevel(enum TdTraceOutputId id, enum TdTraceLevel level);


/**
 * Enables or disables a single trace output.
 *
 * @param id - Trace output ID
 * @param enabled - 1 to enable output, 0 otherwise
 */
void td_setOutputEnabled(enum TdTraceOutputId id, int enabled);

#ifndef TD_DISABLED
	#define LOGV(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_VERBOSE, fmt, ##__VA_ARGS__); }while(0)
	#define LOGD(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_DEBUG, fmt, ##__VA_ARGS__); }while(0)
	#define LOGI(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_INFO, fmt, ##__VA_ARGS__); }while(0)
	#define LOGW(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_WARNING, fmt, ##__VA_ARGS__); }while(0)
	#define LOGE(fmt, ...) do{ td_logMessage(TD_TRACE_TAG, eTD_LVL_ERROR, fmt, ##__VA_ARGS__); }while(0)
#else
	#define LOGV(fmt, ...)
	#define LOGD(fmt, ...)
	#define LOGI(fmt, ...)
	#define LOGW(fmt, ...)
	#define LOGE(fmt, ...)
#endif



#define TRACEV(fmt, ...) LOGV( "{" __FILE__ " - " __FUNCTION__ " @ %d } " fmt, __LINE__, __VA_ARGS__)
#define TRACED(fmt, ...) LOGD( "{" __FILE__ " - " __FUNCTION__ " @ %d } " fmt, __LINE__, __VA_ARGS__)
#define TRACEI(fmt, ...) LOGI( "{" __FILE__ " - " __FUNCTION__ " @ %d } " fmt, __LINE__, __VA_ARGS__)
#define TRACEW(fmt, ...) LOGW( "{" __FILE__ " - " __FUNCTION__ " @ %d } " fmt, __LINE__, __VA_ARGS__)
#define TRACEE(fmt, ...) LOGE( "{" __FILE__ " - " __FUNCTION__ " @ %d } " fmt, __LINE__, __VA_ARGS__)

#define TD_ASSERT(val, fmt, ...) do{ if(!(val)){ TRACEE("Assertion failed: \"" fmt "\"", ##__VA_ARGS__); abort();} }while(0)
#define TD_ASSERT_LT(val1, val2, fmt, ...) TD_ASSERT(val1 < val2, fmt, __VA_ARGS__)
#define TD_ASSERT_LTE(val1, val2, fmt, ...) TD_ASSERT(val1 <= val2, fmt, __VA_ARGS__)
#define TD_ASSERT_GT(val1, val2, fmt, ...) TD_ASSERT(val1 > val2, fmt, __VA_ARGS__)
#define TD_ASSERT_GTE(val1, val2, fmt, ...) TD_ASSERT(val1 >= val2, fmt, __VA_ARGS__)

#ifndef TD_TRACE_TAG
	#define TD_TRACE_TAG ("")
#endif

#ifdef __cplusplus
} // </extern "C"{>
#endif

#endif // </TDTD_H>
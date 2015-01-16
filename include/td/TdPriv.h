#ifndef TDPRIV_H
#define TDPRIV_H

#include "td/td.h"

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

typedef enum TdColorType_t {
	eTD_COLOR_BACKGROUND,
	eTD_COLOR_FOREGROUND,
} TdColorType;

typedef enum TdColor_t {
	eTD_COLOR_RED,
	eTD_COLOR_GREEN,
	eTD_COLOR_BLUE,
	eTD_COLOR_CYAN,
	eTD_COLOR_MAGENTA,
	eTD_COLOR_YELLOW,
	eTD_COLOR_WHITE,
	eTD_COLOR_BLACK,
} TdColor;

#endif

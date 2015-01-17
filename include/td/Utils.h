#ifndef TDUTILS_H
#define TDUTILS_H

#include <td/td.h>
#include <td/TdPriv.h>

#include <string.h>
#include <stdarg.h>
#include <time.h>

inline void td_getCurrentTime(const tdchar* format, tdchar* result){
	time_t rawTime;
	struct tm timeInfo;

	time(&rawTime);


	localtime_s(&timeInfo, &rawTime);

	char bfr[128];

	strftime(bfr, 128, format, &timeInfo);
#ifdef TD_UNICODE
	mbstowcs(result, bfr, 128);
#else
	strcpy(result, bfr);
#endif
}

inline tdchar* td_format(const  tdchar* format, ...){
	va_list args;

	va_start(args, format);

	tdchar* res = td_format(format, args);

	va_end(args);

	return res;
}

inline tdchar* td_format(const  tdchar* format, va_list args){
	// TODO
	tdchar* message = new tdchar[1024];

	td_vsprintf(message, 1024, format, args);

	return message;
}

#endif

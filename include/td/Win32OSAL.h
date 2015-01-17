#ifndef TDWIN32_H
#define TDWIN32_H

#if defined(_WIN32)

#include <td/IOSAL.h>

#include <stdint.h>
#include <Windows.h>

class Win32OSAL : public IOSAL{
public:
	Win32OSAL();

	~Win32OSAL();

	TdError lock();

	TdError unlock();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	TdError resetConsoleColor();

	static Win32OSAL* getInstance();

private:
	TdError setColor(TdColor color, TdColorType type, bool intense);

private:
	CRITICAL_SECTION mCritSec;
	WORD mDefaultAttribs;

private:
	static Win32OSAL* sInstance;
};

#endif

#endif

#ifndef TDWIN32_H
#define TDWIN32_H

#if defined(WIN32)

#include <stdint.h>

#include <Windows.h>

#define	WIN32_COLOR_RED	( 0x04 )
#define	WIN32_COLOR_GREEN		( 0x02 )
#define	WIN32_COLOR_BLUE		( 0x01 )
#define	WIN32_COLOR_CYAN		( WIN32_COLOR_BLUE | WIN32_COLOR_GREEN )
#define	WIN32_COLOR_MAGENTA	( WIN32_COLOR_RED | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_YELLOW		( WIN32_COLOR_GREEN | WIN32_COLOR_RED )
#define	WIN32_COLOR_WHITE		( WIN32_COLOR_RED | WIN32_COLOR_GREEN | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_BLACK		( 0x00 )
#define WIN32_COLOR_INTENSE	( 0x08 )


class Win32OSAL : public IOSAL{
public:
	TdError lock(){
		// TODO
		return eTD_NO_ERROR;
	}

	TdError unlock(){
		// TODO
		return eTD_NO_ERROR;
	}

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold){
		// TODO
		return eTD_NO_ERROR;
	}

	TdError resetConsoleColor(){
		// TODO
		return eTD_NO_ERROR;
	}

private:
	CRITICAL_SECTION gCritSec;

	int gCritSecInitialized = 0;
};

#endif

#endif

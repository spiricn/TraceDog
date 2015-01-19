#if defined(_WIN32)

#include <td/Win32OSAL.h>

#define	WIN32_COLOR_RED	( 0x04 )
#define	WIN32_COLOR_GREEN ( 0x02 )
#define	WIN32_COLOR_BLUE ( 0x01 )
#define	WIN32_COLOR_CYAN ( WIN32_COLOR_BLUE | WIN32_COLOR_GREEN )
#define	WIN32_COLOR_MAGENTA	( WIN32_COLOR_RED | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_YELLOW ( WIN32_COLOR_GREEN | WIN32_COLOR_RED )
#define	WIN32_COLOR_WHITE ( WIN32_COLOR_RED | WIN32_COLOR_GREEN | WIN32_COLOR_BLUE )
#define	WIN32_COLOR_BLACK ( 0x00 )
#define WIN32_COLOR_INTENSE	( 0x08 )


Win32OSAL* Win32OSAL::sInstance = NULL;

Win32OSAL::Win32OSAL(){
	InitializeCriticalSection(&mCritSec);

	// Get the default console attributes so we can reset it later (there's probably a better way of doing this)
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(stdoutHandle, &info);

	mDefaultAttribs = info.wAttributes;
}

Win32OSAL::~Win32OSAL(){
	DeleteCriticalSection(&mCritSec);
}

Win32OSAL* Win32OSAL::getInstance(){
	if (!sInstance){
		sInstance = new Win32OSAL;
	}

	return sInstance;
}

TdError Win32OSAL::lock(){
	EnterCriticalSection(&mCritSec);

	return eTD_NO_ERROR;
}

TdError Win32OSAL::unlock(){
	LeaveCriticalSection(&mCritSec);

	return eTD_NO_ERROR;
}

TdError Win32OSAL::setColor(TdColor color, TdColorType type, bool intense){
	BOOL rc = FALSE;
	WORD attribs = 0;

	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;

	rc = GetConsoleScreenBufferInfo(stdoutHandle, &info);
	if (!rc){
		return eTD_ERROR;
	}

	// First niblet holds the information about foreground color (RGBI) while the second
	// one holds the information about background
	attribs = info.wAttributes;

	int colorCode = 0;

	if (color == eTD_COLOR_RED){
		colorCode = WIN32_COLOR_RED;
	}
	else if (color == eTD_COLOR_GREEN){
		colorCode = WIN32_COLOR_GREEN;
	}
	else if (color == eTD_COLOR_BLUE){
		colorCode = WIN32_COLOR_BLUE;
	}
	else if (color == eTD_COLOR_CYAN){
		colorCode = WIN32_COLOR_CYAN;
	}
	else if (color == eTD_COLOR_MAGENTA){
		colorCode = WIN32_COLOR_MAGENTA;
	}
	else if (color == eTD_COLOR_YELLOW){
		colorCode = WIN32_COLOR_YELLOW;
	}
	else if (color == eTD_COLOR_WHITE){
		colorCode = WIN32_COLOR_WHITE;
	}
	else if (color == eTD_COLOR_BLACK){
		colorCode = WIN32_COLOR_BLACK;
	}

	if (intense){
		colorCode |= WIN32_COLOR_INTENSE;
	}

	if (type == eTD_COLOR_BACKGROUND){
		attribs &= 0xFFFF00FF;
		attribs |= (WORD)((colorCode & 0xF) << 4);
	}
	else{
		attribs &= 0xFFFFFF00;
		attribs |= (WORD)(colorCode & 0xF);
	}

	rc = SetConsoleTextAttribute(stdoutHandle, attribs);

	return rc ? eTD_NO_ERROR : eTD_ERROR;
}

TdError Win32OSAL::setColor(TdColor textColor, bool textIntense, TdColor backgroundColor, bool backgroundIntense){
	TdError rc;

	resetConsoleColor();

	rc = setColor(textColor, eTD_COLOR_FOREGROUND, textIntense);
	if (rc){
		return rc;
	}

	rc = setColor(backgroundColor, eTD_COLOR_BACKGROUND, backgroundIntense);
	if (rc){
		return rc;
	}

	return rc;
}

TdError Win32OSAL::resetConsoleColor(){
	HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return SetConsoleTextAttribute(stdoutHandle, mDefaultAttribs) ? eTD_NO_ERROR : eTD_ERROR;
}

#endif /* _WIN32 */


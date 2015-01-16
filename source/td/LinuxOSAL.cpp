#include <td/LinuxOSAL.h>

#include <string.h>

#define ANSI_CLR_RESET "\x1b[0m"

#define ANSI_CLR_FG_RED "31"
#define ANSI_CLR_FG_GREEN "32"
#define ANSI_CLR_FG_BLUE "34"
#define ANSI_CLR_FG_CYAN "36"
#define ANSI_CLR_FG_MAGENTA "35"
#define ANSI_CLR_FG_YELLOW "33"
#define ANSI_CLR_FG_WHITE "37"
#define ANSI_CLR_FG_BLACK "30"

#define ANSI_CLR_BG_RED "41"
#define ANSI_CLR_BG_GREEN "42"
#define ANSI_CLR_BG_BLUE "44"
#define ANSI_CLR_BG_CYAN "46"
#define ANSI_CLR_BG_MAGENTA "45"
#define ANSI_CLR_BG_YELLOW "43"
#define ANSI_CLR_BG_WHITE "47"
#define ANSI_CLR_BG_BLACK "40"

LinuxOSAL::LinuxOSAL(){
	pthread_mutex_init(&mMutex, NULL);
}

LinuxOSAL::~LinuxOSAL(){
	pthread_mutex_destroy(&mMutex);
}

TdError LinuxOSAL::lock(){
	return pthread_mutex_lock(&mMutex) ? eTD_ERROR : eTD_NO_ERROR;
}

TdError LinuxOSAL::unlock(){
	return pthread_mutex_unlock(&mMutex) ? eTD_ERROR : eTD_NO_ERROR;
}

TdError LinuxOSAL::setColor(TdColor textColor, bool textIntense, TdColor backgroundColor, bool backgroundIntense){
	TdError rc;

	resetConsoleColor();

	rc = setConsoleColor(textColor, eTD_COLOR_FOREGROUND, textIntense);
	if(rc){
		return rc;
	}

	rc = setConsoleColor(backgroundColor, eTD_COLOR_BACKGROUND, backgroundIntense);
	if(rc){
		return rc;
	}

	return rc;
}

TdError LinuxOSAL::setConsoleColor(TdColor color, TdColorType type, bool intense){
	char code[64] = "\x1b[";

	if(color == eTD_COLOR_RED){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_RED : ANSI_CLR_BG_RED);
	}
	else if(color == eTD_COLOR_GREEN){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_GREEN : ANSI_CLR_BG_GREEN);
	}
	else if(color == eTD_COLOR_BLUE){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_BLUE : ANSI_CLR_BG_BLUE);
	}
	else if(color == eTD_COLOR_CYAN){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_CYAN : ANSI_CLR_BG_CYAN);
	}
	else if(color == eTD_COLOR_MAGENTA){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_MAGENTA : ANSI_CLR_BG_MAGENTA);
	}
	else if(color == eTD_COLOR_YELLOW){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_YELLOW : ANSI_CLR_BG_YELLOW);
	}
	else if(color == eTD_COLOR_WHITE){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_WHITE : ANSI_CLR_BG_WHITE);
	}
	else if(color == eTD_COLOR_BLACK){
		strcat(code, type == eTD_COLOR_FOREGROUND ? ANSI_CLR_FG_BLACK : ANSI_CLR_BG_BLACK);
	}
	else{
		return eTD_ERROR;
	}

	if(intense){
		strcat(code, ";1m");
	}
	else{
		strcat(code, "m");
	}

	fprintf(stdout, "%s", code);

	return eTD_NO_ERROR;
}

TdError LinuxOSAL::resetConsoleColor(){
	fprintf(stdout, ANSI_CLR_RESET);

	return eTD_NO_ERROR;
}

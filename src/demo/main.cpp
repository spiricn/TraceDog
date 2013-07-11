//#define TD_UNICODE

#include "td/td.h"

#include <iostream>

#ifdef TD_UNICODE
	#pragma comment(lib, "td_unicode.lib")
#else
	#pragma comment(lib, "td.lib")
#endif

#define TD_TRACE_TAG ( TD_TEXT("ModuleTag") )

void callbackFnc(const tdchar* tag, enum TdTraceLevel level, const tdchar* message){
#ifdef TD_UNICODE
	std::wcout
#else
	std::cout 
#endif
	<< "callback: tag=\"" << tag << "\" level=" << level << " message=\"" << message << "\"" << std::endl;
}

int main(){
	// Setup a log file
	FILE* test = fopen("log.html", "wb");
	td_setFileOutput(test);

	// Set HTML format and write HTML header to file
	td_setFileOutputType(eTD_FILE_HTML);
	

	// Setup a user calblack function
	td_setCallbackFnc(callbackFnc);

	// Print some logs
	LOGV(TD_TEXT("%s level test: \"%d\""), TD_TEXT("Verbose"), 12345);
	LOGD(TD_TEXT("%s level test: \"%d\""), TD_TEXT("Debug"), 12345);
	LOGI(TD_TEXT("%s level test: \"%d\""), TD_TEXT("Info"), 12345);
	LOGW(TD_TEXT("%s level test: \"%d\""), TD_TEXT("Warning"), 12345);
	LOGE(TD_TEXT("%s level test: \"%d\""), TD_TEXT("Error"), 12345);

	// Print only errors in the console
	td_setOutputLevel(eTD_OUTPUT_CONSOLE, eTD_LVL_ERROR);
	LOGV(TD_TEXT("This will not be seen on the console"));
	LOGD(TD_TEXT("This will not be seen on the console"));
	LOGI(TD_TEXT("This will not be seen on the console"));
	LOGW(TD_TEXT("This will not be seen on the console"));
	LOGE(TD_TEXT("This WILL be seen on the console"));

	// Disable user callback function
	td_setOutputEnabled(eTD_OUTPUT_USER, 0);
	LOGE(TD_TEXT("The user will NOT receive this"));

	// Disable file and console output aswell
	td_setOutputEnabled(eTD_OUTPUT_CONSOLE, 0);
	td_setOutputEnabled(eTD_OUTPUT_FILE, 0);
	LOGE(TD_TEXT("Nobody will see this"));

	// Cleanup
	td_setFileOutput(NULL);
	td_setCallbackFnc(NULL);
	fclose(test);

	std::cout << std::endl << std::endl << std::endl;

	// Enable console output again
	td_setOutputEnabled(eTD_OUTPUT_CONSOLE, 1);
	td_setOutputLevel(eTD_OUTPUT_CONSOLE, eTD_LVL_VERBOSE);


	TRACEV(TD_TEXT("Tracing something boring %d"), 123);

	TRACEE(TD_TEXT("Something failed, error code = %#x"), 12345);

	// Assertion example
	TD_ASSERT(true, TD_TEXT("Something very bad happened (error code = %#x)"), 1231234);

	return 0;
}
#include "td/td.h"

#include <iostream>


#define TD_TRACE_TAG "ModuleTag"

void callbackFnc(const tdchar* tag, enum TdTraceLevel level, const tdchar* message){
	std::cout << "callback: tag=\"" << tag << "\" level=" << level << " message=\"" << message << "\"" << std::endl;
}

int main(){
	// Setup a log file
	FILE* test = fopen("test.log", "wb");
	td_setFileOutput(test);

	// Setup a user calblack function
	td_setCallbackFnc(callbackFnc);

	// Print some logs
	LOGV("%s level test: \"%d\"", "Verbose", 12345);
	LOGD("%s level test: \"%d\"", "Debug", 12345);
	LOGI("%s level test: \"%d\"", "Info", 12345);
	LOGW("%s level test: \"%d\"", "Warning", 12345);
	LOGE("%s level test: \"%d\"", "Error", 12345);

	// Print only errors in the console
	td_setOutputLevel(eTD_OUTPUT_CONSOLE, eTD_LVL_ERROR);
	LOGV("This will not be seen");
	LOGD("This will not be seen");
	LOGI("This will not be seen");
	LOGW("This will not be seen");
	LOGE("Thiss WILL be seen");

	// Disable user callback function
	td_setOutputEnabled(eTD_OUTPUT_USER, 0);
	LOGE("The user will NOT receive this");

	// Disable file and console output aswell
	td_setOutputEnabled(eTD_OUTPUT_CONSOLE, 0);
	td_setOutputEnabled(eTD_OUTPUT_FILE, 0);
	LOGE("Nobody will see this");

	// Cleanup
	td_setFileOutput(NULL);
	td_setCallbackFnc(NULL);
	fclose(test);


	// Enable console output again
	td_setOutputEnabled(eTD_OUTPUT_CONSOLE, 1);
	td_setOutputLevel(eTD_OUTPUT_CONSOLE, eTD_LVL_VERBOSE);

	TRACEV("Tracing something %d", 123);

	TRACEE("Something failed, error code = %#x", 12345);

	// Assertion example
	TD_ASSERT(true, "Something very bad happened (error code = %#x)", 1231234);
	
}
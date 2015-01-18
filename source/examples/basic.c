#include <td/td.h>

// First we need to define our module tag (if we're using TD wrapper macros)
#undef TD_TRACE_TAG
#define TD_TRACE_TAG "MyModule"

int main(){
	// Note that the console output is created by default

	// Create a formatted HTML file output
	TdOutputHandle html = td_createFileOutput("log.html", eTD_FMT_HTML);

	// Create a plain text output
	TdOutputHandle plain = td_createFileOutput("log.txt", eTD_FMT_PLAIN_TEXT);

	// Basic log macros
	LOGV("Verbose LOG");

	LOGD("Debug LOG");

	LOGI("Info LOG");

	LOGW("Warning LOG");

	LOGE("Error LOG");

	// Tracing macro
	TRACEV("Verbose TRACE");

	TRACED("Debug TRACE");

	TRACEI("Info TRACE");

	TRACEW("Warning TRACE");

	TRACEE("Error TRACE");

	// Destroy previously created outputs
	td_destroyOutput(html);

	td_destroyOutput(plain);

	system("pause");

	return 0;
}

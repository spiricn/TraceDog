#include <td/td.h>

#include "td/TdImpl.h"

static Td* gTd = new Td;

TdError td_setCallbackFnc(td_callbackFcn fnc, void* userData){
	return gTd->setCallbackFnc(fnc, userData);
}

TdError td_setOutputLevel(TdOutputHandle handle, TdTraceLevel level){
	return gTd->setOutputLevel(handle, level);
}

TdError td_logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* message, ...){
	va_list argList;

	va_start(argList, message);

	TdError rc = gTd->logMessage(tag, level, message, argList);

	va_end(argList);

	return rc;
}

TdOutputHandle td_createFileOutput(const tdchar* path, TdFileFormat fileFormat){
	return gTd->createFileOutput(path, fileFormat);
}

TdOutputHandle td_createConsoleOutput(){
	return gTd->createConsoleOutput();
}

TdError td_destroyOutput(TdOutputHandle handle){
	return gTd->destroyOutput(handle);
}

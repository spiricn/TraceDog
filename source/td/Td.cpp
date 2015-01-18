#include "td/TdImpl.h"

TdError td_setCallbackFnc(td_callbackFcn fnc, void* userData){
	return TdImpl::getInstance()->setCallbackFnc(fnc, userData);
}

TdError td_setOutputLevel(TdOutputHandle handle, TdTraceLevel level){
	return TdImpl::getInstance()->setOutputLevel(handle, level);
}

TdError td_logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* message, ...){
	va_list argList;

	va_start(argList, message);

	TdError rc = TdImpl::getInstance()->logMessage(tag, level, message, argList);

	va_end(argList);

	return rc;
}

TdOutputHandle td_createFileOutput(const tdchar* path, TdFileFormat fileFormat){
	return TdImpl::getInstance()->createFileOutput(path, fileFormat);
}

TdOutputHandle td_createConsoleOutput(){
	return TdImpl::getInstance()->createConsoleOutput();
}

TdError td_destroyOutput(TdOutputHandle handle){
	return TdImpl::getInstance()->destroyOutput(handle);
}

TdOutputHandle td_getDefaultOutput(){
	return TdImpl::getInstance()->getDefaultOutput();

}
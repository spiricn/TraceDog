#include "td/TdImpl.h"

TdImpl* TdImpl::sInstance = NULL;

TdImpl::TdImpl(){
	mCallback.fnc = NULL;
	mCallback.data = NULL;
}

TdError TdImpl::logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* format, va_list args){
	IOSAL::getInstance()->lock();

	tdchar* message = td_format(format, args);

	tdchar* linePtr = td_strtok(message, TD_TEXT("\n"));

	while(linePtr != NULL){
		// Send user callback first
		if(mCallback.fnc){
			mCallback.fnc(tag, level, message, mCallback.data);
		}

		// Handle other outputs
		for(OutputList::iterator iter=mOutputs.begin(); iter!=mOutputs.end(); iter++){
			if (static_cast<int>(level) >= static_cast<int>((*iter)->getLevel())){
				logMessage(tag, linePtr, level, *iter);
			}
		}

		// Next line
		linePtr = td_strtok(NULL, TD_TEXT("\n"));
	}

	delete[] message;

	IOSAL::getInstance()->unlock();

	return eTD_NO_ERROR;
}


TdError TdImpl::logMessage(const tdchar* tag, const tdchar* message, TdTraceLevel level, AOutput* output){
	output->onMessageStart();

	const TdColor levelColor = Formatter::getLevelColor(level);

	const tdchar levelSymbol = Formatter::getLevelSymbol(level);

	const tdchar levelStr[2] = {levelSymbol, 0};

	output->resetColor();

	{
		// Write time stamp
		tdchar timeStamp[64];
		td_getCurrentTime("%H:%M:%S", timeStamp);

		output->setColor(eTD_COLOR_WHITE, false, eTD_COLOR_BLACK, false);
		output->writeFmt(TD_TEXT("[") TD_STR_FMT TD_TEXT(" "), timeStamp);
	}

	{
		// Log level
		output->setColor(eTD_COLOR_WHITE, true, levelColor, false);
		output->writeFmt(TD_TEXT(" ") TD_STR_FMT TD_TEXT(" "), levelStr);
	}

	{
		// Log tag
		output->setColor(eTD_COLOR_WHITE, false, eTD_COLOR_BLACK, false);
		output->writeFmt(TD_TEXT(" ") TD_STR_FMT TD_TEXT("]: "), tag);
	}

	{
		// Message line
		output->setColor(levelColor,  level != eTD_LVL_VERBOSE, eTD_COLOR_BLACK, false);
		output->writeFmt(TD_STR_FMT, message);
	}

	output->writeFmt(TD_TEXT("\r\n"));

	output->resetColor();

	output->onMessageEnd();

	return eTD_NO_ERROR;
}


TdError TdImpl::setCallbackFnc(td_callbackFcn fnc, void* userData){
	IOSAL::getInstance()->lock();

	mCallback.fnc = fnc;
	mCallback.data = userData;

	IOSAL::getInstance()->unlock();

	return eTD_NO_ERROR;
}

TdError TdImpl::setOutputLevel(TdOutputHandle handle, TdTraceLevel level){
	IOSAL::getInstance()->lock();

	AOutput* output = static_cast<AOutput*>(handle);

	if (output == NULL){
		for(OutputList::iterator iter = mOutputs.begin(); iter!=mOutputs.end(); iter++){
			(*iter)->setLevel(level);
		}
	}
	else{
		output->setLevel(level);
	}
	

	IOSAL::getInstance()->unlock();

	return eTD_NO_ERROR;
}

TdOutputHandle TdImpl::createFileOutput(const tdchar* path, TdFileFormat fileFormat){
	IOSAL::getInstance()->lock();

	AOutput* output = NULL;

	if(fileFormat == eTD_FMT_PLAIN_TEXT){
		output = new PlainFileOutput(path);
	}
	else if(fileFormat == eTD_FMT_HTML){
		output = new HTMLFileOutput(path);
	}
	else{
		IOSAL::getInstance()->unlock();

		return NULL;
	}

	attachOutput(output);

	IOSAL::getInstance()->unlock();

	return static_cast<TdOutputHandle>(output);
}

TdOutputHandle TdImpl::createConsoleOutput(){
	IOSAL::getInstance()->lock();

	AOutput* output = new ConsoleOutput();

	attachOutput(output);

	IOSAL::getInstance()->unlock();

	return static_cast<TdOutputHandle>(output);
}

TdError TdImpl::destroyOutput(TdOutputHandle handle){
	IOSAL::getInstance()->lock();

	AOutput* output = static_cast<AOutput*>(handle);

	for(OutputList::iterator iter=mOutputs.begin(); iter!=mOutputs.end(); iter++){
		if(*iter == output){
			mOutputs.erase(iter);
			break;
		}
	}

	delete output;

	IOSAL::getInstance()->unlock();

	return eTD_NO_ERROR;
}


 TdImpl* TdImpl::getInstance(){
	if(!sInstance){
		sInstance = new TdImpl;
	}

	return sInstance;
}
void TdImpl::attachOutput(AOutput* output){
	output->onMessageStart();

	mOutputs.push_back(output);

	output->onMessageEnd();
}

#ifndef TDIMPL_H
#define TDIMPL_H

#include <stdarg.h>
#include <td/IOSAL.h>
#include <td/IOutput.h>
#include <td/ConsoleOutput.h>
#include <td/PlainFileOutput.h>
#include <td/HTMLFileOutput.h>

#include <vector>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <td/TdPriv.h>
#include <td/Utils.h>
#include <td/Formatter.h>

class Td{
public:
	Td(){
		mCallback.fnc = NULL;
		mCallback.data = NULL;
	}

	TdError logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* format, va_list args){
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
				logMessage(tag, linePtr, level, *iter);
			}

			// Next line
			linePtr = td_strtok(NULL, TD_TEXT("\n"));
		}

		delete[] message;

		IOSAL::getInstance()->unlock();

		return eTD_NO_ERROR;
	}


	TdError logMessage(const tdchar* tag, const tdchar* message, TdTraceLevel level, IOutput* output){
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


	TdError setCallbackFnc(td_callbackFcn fnc, void* userData){
		IOSAL::getInstance()->lock();

		mCallback.fnc = fnc;
		mCallback.data = userData;

		IOSAL::getInstance()->unlock();

		return eTD_NO_ERROR;
	}

	TdError setOutputLevel(TdOutputHandle handle, TdTraceLevel level){
		IOSAL::getInstance()->lock();

		IOSAL::getInstance()->unlock();

		return eTD_NO_ERROR;
	}

	TdOutputHandle createFileOutput(const tdchar* path, TdFileFormat fileFormat){
		IOSAL::getInstance()->lock();

		IOutput* output = NULL;

		TdOutputHandle handle = generateHandle();

		if(fileFormat == eTD_FMT_PLAIN_TEXT){
			output = new PlainFileOutput(path, handle);
		}
		else if(fileFormat == eTD_FMT_HTML){
			output = new HTMLFileOutput(path, handle);
		}
		else{
			return TD_INVALID_OUTPUT_HANDLE;
		}

		attachOutput(output);

		IOSAL::getInstance()->unlock();

		return handle;
	}

	TdOutputHandle createConsoleOutput(){
		IOSAL::getInstance()->lock();

		IOutput* output = new ConsoleOutput(generateHandle());

		attachOutput(output);

		TdOutputHandle res = output->getHandle();

		IOSAL::getInstance()->unlock();

		return res;
	}

	TdError destroyOutput(TdOutputHandle handle){
		IOSAL::getInstance()->lock();

		IOutput* output = findOutput(handle);

		if(output == NULL){
			return eTD_ERROR;
		}

		for(OutputList::iterator iter=mOutputs.begin(); iter!=mOutputs.end(); iter++){
			if((*iter)->getHandle() == handle){
				mOutputs.erase(iter);
				break;
			}
		}

		delete output;

		IOSAL::getInstance()->unlock();

		return eTD_NO_ERROR;
	}


	static Td* getInstance(){
		if(!sInstance){
			sInstance = new Td;
		}

		return sInstance;
	}

private:
	static Td* sInstance;

private:
	void attachOutput(IOutput* output){
		output->onMessageStart();

		mOutputs.push_back(output);

		output->onMessageEnd();
	}

	TdOutputHandle generateHandle(){
		TdOutputHandle handle = 0;

		while(true){
			if(findOutput(handle)){
				++handle;
			}
			else{
				break;
			}
		}

		return handle;
	}

	IOutput* findOutput(TdOutputHandle handle){
		for(OutputList::iterator iter=mOutputs.begin(); iter!=mOutputs.end(); iter++){
			if((*iter)->getHandle() == handle){
				return *iter;
			}
		}

		return NULL;
	}

private:
	struct{
		td_callbackFcn fnc;
		void* data;
	} mCallback;

	typedef std::vector<IOutput*> OutputList;

	OutputList mOutputs;
};

#endif

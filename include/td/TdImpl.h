#ifndef TDIMPL_H
#define TDIMPL_H

#include <stdarg.h>
#include <td/IOSAL.h>
#include <td/AOutput.h>
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

class TdImpl{
public:
	TdImpl();

	~TdImpl();

	TdError logMessage(const tdchar* tag, TdTraceLevel level, const tdchar* format, va_list args);

	TdError logMessage(const tdchar* tag, const tdchar* message, TdTraceLevel level, AOutput* output);

	TdError setCallbackFnc(td_callbackFcn fnc, void* userData);

	TdError setOutputLevel(TdOutputHandle handle, TdTraceLevel level);

	TdOutputHandle createFileOutput(const tdchar* path, TdFileFormat fileFormat);

	TdOutputHandle createConsoleOutput();

	TdError destroyOutput(TdOutputHandle handle);

	TdOutputHandle getDefaultOutput() const;

	static TdImpl* getInstance();

private:
	typedef std::vector<AOutput*> OutputList;

private:
	void attachOutput(AOutput* output);

private:
	struct{
		td_callbackFcn fnc;
		void* data;
	} mCallback;

	OutputList mOutputs;
	IOSAL* mOsal;
	TdOutputHandle mDefaultOutput;

private:
	static TdImpl* sInstance;
};

#endif

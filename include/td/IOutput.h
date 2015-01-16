#ifndef IOUTPUT_H_
#define IOUTPUT_H_

#include <td/td.h>
#include <td/TdPriv.h>
#include <td/Utils.h>

class IOutput{
public:
	IOutput(TdOutputHandle handle) : mHandle(handle){
	}

	virtual ~IOutput(){
	}

	virtual void onMessageStart(){
	}

	virtual void onMessageEnd(){
	}

	virtual TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold) = 0;

	virtual TdError resetColor() = 0;

	virtual void write(const tdchar* string) = 0;

	void writeFmt(const tdchar* format, ...){
		va_list args;
		va_start(args, format);

		tdchar* message = td_format(format, args);

		va_end(args);

		write(message);

		delete[] message;
	}

	TdOutputHandle getHandle() const{
		return mHandle;
	}

private:
	TdOutputHandle mHandle;
};


#endif /* IOUTPUT_H_ */

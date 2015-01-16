#ifndef IOUTPUT_H_
#define IOUTPUT_H_

#include <td/td.h>
#include <td/TdPriv.h>
#include <td/Utils.h>

class AOutput{
public:
	virtual ~AOutput(){
	}

	virtual void onMessageStart();

	virtual void onMessageEnd();

	void writeFmt(const tdchar* format, ...);

	virtual TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold) = 0;

	virtual TdError resetColor() = 0;

	virtual void write(const tdchar* string) = 0;
};


#endif /* IOUTPUT_H_ */

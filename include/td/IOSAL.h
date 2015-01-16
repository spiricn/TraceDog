#ifndef IOSAL_H_
#define IOSAL_H_

#include <td/td.h>
#include <td/TdPriv.h>

class IOSAL{
public:
	virtual ~IOSAL(){
	}

	virtual TdError lock() = 0;

	virtual TdError unlock() = 0;

	virtual TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold) = 0;

	virtual TdError resetConsoleColor() = 0;

	static IOSAL* getInstance();
};


#endif /* IOSAL_H_ */

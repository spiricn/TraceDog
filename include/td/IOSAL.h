#ifndef TDIOSAL_H
#define TDIOSAL_H

#include "td/td.h"
#include "td/TdPriv.h"

class IOSAL{
public:
	virtual ~IOSAL(){
	}

	virtual TdError lock() = 0;

	virtual TdError unlock() = 0;

	virtual TdError setColor(TdColor textColor, bool textIntense, TdColor backgroundColor, bool backgroundIntense) = 0;

	virtual TdError resetConsoleColor() = 0;

	static IOSAL* getInstance();
};

#endif /* TDIOSAL_H */

#ifndef TDAOUTPUT_H
#define TDAOUTPUT_H

#include "td/td.h"
#include "td/TdPriv.h"
#include "td/Utils.h"

class AOutput{
public:
	AOutput();

	virtual ~AOutput();

	virtual TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold) = 0;

	virtual TdError resetColor() = 0;

	virtual void write(const tdchar* string) = 0;

	virtual void onMessageStart();

	virtual void onMessageEnd();

	void writeFmt(const tdchar* format, ...);

	void setLevel(TdTraceLevel level);

	TdTraceLevel getLevel() const;

private:
	TdTraceLevel mLevel;
};

#endif /* TDAOUTPUT_H */


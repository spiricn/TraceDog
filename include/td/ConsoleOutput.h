#ifndef TDCONSOLEOUTPUT_H
#define TDCONSOLEOUTPUT_H

#include <td/AOutput.h>

class ConsoleOutput : public AOutput{
public:
	ConsoleOutput();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();
};

#endif /* TDCONSOLEOUTPUT_H */


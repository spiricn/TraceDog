#ifndef CONSOLEOUTPUT_H_
#define CONSOLEOUTPUT_H_

#include <td/AOutput.h>

class ConsoleOutput : public AOutput{
public:
	ConsoleOutput();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();
};

#endif /* CONSOLEOUTPUT_H_ */

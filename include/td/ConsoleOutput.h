#ifndef CONSOLEOUTPUT_H_
#define CONSOLEOUTPUT_H_

#include <td/IOutput.h>

class ConsoleOutput : public IOutput{
public:
	ConsoleOutput(TdOutputHandle handle);

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();
};

#endif /* CONSOLEOUTPUT_H_ */

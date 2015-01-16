#ifndef PLAINFILEOUTPUT_H
#define PLAINFILEOUTPUT_H

#include <td/IOutput.h>

class PlainFileOutput : public IOutput{
public:
	PlainFileOutput(const char* path, TdOutputHandle handle);

	~PlainFileOutput();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();

private:
	FILE* mFile;
};

#endif /* CONSOLEOUTPUT_H_ */

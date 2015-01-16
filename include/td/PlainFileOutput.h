#ifndef PLAINFILEOUTPUT_H
#define PLAINFILEOUTPUT_H

#include <td/AOutput.h>

class PlainFileOutput : public AOutput{
public:
	PlainFileOutput(const char* path);

	~PlainFileOutput();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();

private:
	FILE* mFile;
};

#endif /* CONSOLEOUTPUT_H_ */

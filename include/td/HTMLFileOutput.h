#ifndef HTMLFILEOUTPUT_H
#define HTMLFILEOUTPUT_H

#include "td/IOutput.h"
#include <string>

class HTMLFileOutput : public IOutput {
public:
	HTMLFileOutput(const char* path, TdOutputHandle handle);

	~HTMLFileOutput();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	void write(const tdchar* string);

	TdError resetColor();

	void onMessageStart();

	void onMessageEnd();

private:
	FILE* mFile;
	std::string mCurrentColor;
};

#endif

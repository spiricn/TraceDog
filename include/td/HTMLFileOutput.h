#ifndef TDHTMLFILEOUTPUT_H
#define TDHTMLFILEOUTPUT_H

#include "td/AOutput.h"

#include <string>

class HTMLFileOutput : public AOutput {
public:
	HTMLFileOutput(const char* path);

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

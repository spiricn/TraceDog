#include "td/HTMLFileOutput.h"

HTMLFileOutput::HTMLFileOutput(const char* path) : mFile(NULL){
	mFile = fopen(path, "wb");

	// Header
	fprintf(mFile,
			"<!DOCTYPE html>\n"
			"<html>\n"
	);

	// Head & style
	fprintf(mFile,
			"<head>\n"
			"<style>\n"
			"p{\n"
				"white-space: pre-wrap;\n"
				"font-family: monospace;\n"
				"margin: 0;\n"
				"padding:0;\n"
				"font-size: 15px;\n"
				"color: #fff;\n"
				"background-color: #000;\n"
			"}\n"
			"</style>\n"
			"</head>\n"
	);

	// Body start
	fprintf(mFile, "<body bgcolor=\"black\">\n");
}

HTMLFileOutput::~HTMLFileOutput(){
	// Trailer
	fprintf(mFile,
			"</body>\n"
			"</html>\n"
	);

	if(mFile){
		fclose(mFile);
	}
}

void HTMLFileOutput::onMessageStart(){
	fprintf(mFile, "<p>");
}

void HTMLFileOutput::onMessageEnd(){
	fprintf(mFile, "</p>");
}

std::string convertColor(TdColor color, bool intense){
	switch (color) {
	case eTD_COLOR_RED:
		return intense ? "rgb(255, 0, 0)" : "rgb(129, 0, 0)";

	case eTD_COLOR_GREEN:
		return intense ? "rgb(0, 255, 0)" : "rgb(0, 129, 0)";

	case eTD_COLOR_CYAN:
		return intense ? "rgb(0, 255, 255)" : "rgb(0, 129, 129)";

	case eTD_COLOR_MAGENTA:
		return intense ? "rgb(255, 0, 255)" : "rgb(129, 0, 129)";

	case eTD_COLOR_YELLOW:
		return intense ? "rgb(255, 255, 0)" : "rgb(129, 129, 0)";

	case eTD_COLOR_WHITE:
		return intense ? "rgb(255, 255, 2555)" : "rgb(160, 160, 160)";

	case eTD_COLOR_BLACK:
		return intense ? "rgb(0, 0, 0)" : "rgb(0, 0, 0)";
	}
}

TdError HTMLFileOutput::setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold){
	mCurrentColor = "color: " + convertColor(textColor, textBold) + ";"
			+ "background-color: " + convertColor(backgroundColor, backgroundBold) + ";"
			;

	if(textBold){
		mCurrentColor += "font-weight: bold;";
	}

	return eTD_NO_ERROR;
}

void HTMLFileOutput::write(const tdchar* string){
	std::string msg;

	msg = msg + "<span style = \"" + mCurrentColor + "\" >" + string + "</span>";

	fprintf(mFile, "%s", msg.c_str());
}

TdError HTMLFileOutput::resetColor(){
	return eTD_NO_ERROR;
}

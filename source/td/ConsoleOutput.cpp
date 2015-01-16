#include <td/ConsoleOutput.h>
#include <td/IOSAL.h>

#include <stdio.h>

ConsoleOutput::ConsoleOutput(){
}

TdError ConsoleOutput::setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold){
	return IOSAL::getInstance()->setColor(textColor, textBold, backgroundColor, backgroundBold);
}

TdError ConsoleOutput::resetColor() {
	IOSAL::getInstance()->resetConsoleColor();

	return eTD_NO_ERROR;
}

void ConsoleOutput::write(const tdchar* string){
	fprintf(stdout, "%s", string);
}

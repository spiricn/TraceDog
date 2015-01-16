#include <td/PlainFileOutput.h>
#include <td/IOSAL.h>

#include <stdio.h>

PlainFileOutput::PlainFileOutput(const char* path) : mFile(NULL) {
	mFile = fopen(path, "wb");
}

PlainFileOutput::~PlainFileOutput(){
	if(mFile){
		fclose(mFile);
	}
}

TdError PlainFileOutput::setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold){
	// No need for colors in plain text files
	return eTD_NO_ERROR;
}

TdError PlainFileOutput::resetColor() {
	// No need for colors in plain text files
	return eTD_NO_ERROR;
}

void PlainFileOutput::write(const tdchar* string){
	if(mFile != NULL){
		fprintf(mFile, "%s", string);

		fflush(mFile);
	}
}

#include "td/AOutput.h"

void AOutput::onMessageStart(){
}

void AOutput::onMessageEnd(){
}

void AOutput::writeFmt(const tdchar* format, ...){
	va_list args;
	va_start(args, format);

	tdchar* message = td_format(format, args);

	va_end(args);

	write(message);

	delete[] message;
}

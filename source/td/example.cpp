///**
// * @file
// * @author Nikola Spiric <nikola.spiric.ns@gmail.com>
// *
// * @section LICENSE
// *
// * This program is free software; you can redistribute it and/or
// * modify it under the terms of the GNU General Public License as
// * published by the Free Software Foundation; either version 2 of
// * the License, or (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful, but
// * WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// * General Public License for more details at
// * http://www.gnu.org/copyleft/gpl.html
// */
//
////#define TD_UNICODE
//
#include "td/td.h"

#undef TD_TRACE_TAG
#define TD_TRACE_TAG ( TD_TEXT("ModuleTag") )


int main(){
	TdOutputHandle console = td_createConsoleOutput();

	TdOutputHandle html = td_createFileOutput("/home/nikola/desktop/log.html", eTD_FMT_HTML);

	TdOutputHandle plain = td_createFileOutput("/home/nikola/desktop/log.txt", eTD_FMT_PLAIN_TEXT);

	LOGV("Verbose LOG");
	LOGD("Debug LOG");
	LOGI("Info LOG");
	LOGW("Warning LOG");
	LOGE("Error LOG");
	TRACED("Trace LOG");

	td_destroyOutput(console);
	td_destroyOutput(html);
	td_destroyOutput(plain);

	return 0;
}



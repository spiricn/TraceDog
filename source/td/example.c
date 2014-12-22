/**
 * @file
 * @author Nikola Spiric <nikola.spiric.ns@gmail.com>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

//#define TD_UNICODE

#include "td/td.h"

#define TD_TRACE_TAG ( TD_TEXT("ModuleTag") )


int main(){
	LOGV("Verbose log");
	LOGD("Debug log");
	LOGI("Info log");
	LOGW("Warning log");
	LOGE("Error log");

	TRACED("Trace log");

	return 0;
}

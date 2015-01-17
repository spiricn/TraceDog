#ifndef TDFORMATTER_H
#define TDFORMATTER_H

#include "TdPriv.h"

class Formatter{
public:
	static TdColor getLevelColor(TdTraceLevel level){
			return    level == eTD_LVL_DEBUG  ? eTD_COLOR_WHITE
					: level == eTD_LVL_VERBOSE ? eTD_COLOR_WHITE
					: level == eTD_LVL_INFO ? eTD_COLOR_GREEN
					: level == eTD_LVL_WARNING ? eTD_COLOR_YELLOW
					: level == eTD_LVL_ERROR ? eTD_COLOR_RED
					: eTD_COLOR_WHITE;
	}

	static tdchar getLevelSymbol(TdTraceLevel level){
		return    level == eTD_LVL_DEBUG ? 'D'
				: level == eTD_LVL_VERBOSE ? 'V'
				: level == eTD_LVL_INFO ? 'I'
				: level == eTD_LVL_WARNING ? 'W'
				: level == eTD_LVL_ERROR ? 'E'
				: '?';
	}
};

#endif /* TDFORMATTER_H */


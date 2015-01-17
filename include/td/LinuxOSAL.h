#ifndef TDLINUXOSAL_H
#define TDLINUXOSAL_H

#include "td/IOSAL.h"

#include <pthread.h>

class LinuxOSAL : public IOSAL{
public:
	LinuxOSAL();

	~LinuxOSAL();

	TdError lock();

	TdError unlock();

	TdError setColor(TdColor textColor, bool textBold, TdColor backgroundColor, bool backgroundBold);

	TdError resetConsoleColor();

	static LinuxOSAL* getInstance();

private:
	TdError setConsoleColor(TdColor color, TdColorType type, bool intense);

private:
	pthread_mutex_t mMutex;

private:
	static LinuxOSAL* sInstance;
};

#endif /* TDLINUXOSAL_H */


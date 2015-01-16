#include <td/IOSAL.h>

#if defined(__linux__)

#include <td/LinuxOSAL.h>

static LinuxOSAL* gLinuxOSAL = NULL;

#endif

IOSAL* IOSAL::getInstance(){
#if defined(__linux__)
	if(gLinuxOSAL == NULL){
		gLinuxOSAL = new LinuxOSAL;
	}

	return gLinuxOSAL;
#else
	#error Not implemente
	return NULL;
#endif
}

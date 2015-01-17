#include <td/IOSAL.h>

#if defined(__linux__)
#include <td/LinuxOSAL.h>
#endif

#if defined(_WIN32)
#include <td/Win32OSAL.h>
#endif

IOSAL* IOSAL::getInstance(){
#if defined(__linux__)
	return LinuxOSAL::getInstance();
#elif defined(_WIN32)
	return Win32OSAL::getInstance();
#else
	#error Not implemented on this platform.
#endif
}

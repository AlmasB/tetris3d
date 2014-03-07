#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>

/**
	If enabled, most of the classes/structs will have
	a printDebug() function, which prints debug info to std out
	Also debug info will be shown at crucial stages

	Uncomment the following to remove debug code from release build
*/
#define __DEBUG

inline void debug(const char * msg) {
	std::cout << "DEBUG: " << msg << std::endl;
}

#endif
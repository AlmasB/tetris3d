#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>

/**
	If enabled, most of the classes/structs will have
	a printDebug() function, which prints debug info to std out
*/
#define __DEBUG_ENABLED 1

inline static void debug(const char * msg) {
	std::cout << "DEBUG: " << msg << std::endl;
}

#endif
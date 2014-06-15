/**
 * {XCube3D Game Engine}
 * Copyright(C) { 2013-2014 }  {AlmasB} {a.baimagambetov1@uni.brighton.ac.uk}
 *
 * This program is free software; you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include <stack>
#include <string>

/**
	If enabled, most of the classes/structs will have
	a printDebug() function, which prints debug info to std out
	Also debug info will be shown at crucial stages

	Uncomment the following to remove debug code from release build
*/
#define __DEBUG

inline void debug(const char * msg, const char * details = "") {
	std::cout << "DEBUG: " << msg << " " << details << std::endl;
}

inline void debug(const char * msg, int value) {
	std::cout << "DEBUG: " << msg << " " << value << std::endl;
}

class ErrorStack {
	private:
		static std::stack<std::string> errorData;
	public:
		static void push(const std::string & errorMessage);
		static void dump();
};

#endif
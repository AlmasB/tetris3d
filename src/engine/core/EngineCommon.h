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

#ifndef __ENGINE_COMMON_H__
#define __ENGINE_COMMON_H__

#include <exception>

#include "Debug.h"

#define _ENGINE_ERROR_NONE ""

#define safeDelete(ptr) if (ptr) delete ptr

class EngineException : public std::exception {
	private:
		std::string message;

	public:
		/**
		 * @param arg0, arg1 - exception details
		 */
		EngineException(std::string arg0, std::string arg1 = "") : message("EngineException: " + arg0 + " - " + arg1) {
			std::cout << "\t" << message << std::endl;
		}

		EngineException(const char * arg0, const char * arg1) : EngineException(std::string(arg0), std::string(arg1)) {}

		virtual const char* what() const throw() {
			return message.c_str();
		}
};

#endif
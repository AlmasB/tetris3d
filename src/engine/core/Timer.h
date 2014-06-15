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

#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL2/SDL.h>

#define __SECOND 1000
#define __MINUTE (60*__SECOND)

/**
 * Relative timer, used to tell how much time has passed
 * since certain event rather than the actual time
 */
class Timer {
	private:
		Uint32 time;
	public:
		Timer();
		void measure();
		void reset();
		Uint32 getTime();
		Uint32 getElapsed();
};

#endif
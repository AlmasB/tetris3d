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

#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include <string>
#include <thread>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include "../core/EngineCommon.h"
#include "../math/GameMath.h"

enum Key {
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, QUIT, LAST
};

enum Mouse {
	BTN_LEFT, BTN_RIGHT, BTN_LAST
};

static const Uint16 PORT = 55555;	// should be safe

class EventEngine {
	friend class GameEngine;
	private:
		bool running;
		SDL_Event event;
		bool keys[Key::LAST];
		bool buttons[Mouse::BTN_LAST];
		Point2 mouseDPos;

		void updateKeys(const SDL_Keycode &, bool);

		bool remoteEventEnabled;
		std::thread * connThread;

		IPaddress ip, *remoteip;
		TCPsocket server, client;
		char message[8];	// recv buffer;
		int len;
		Uint32 ipaddr;
		Uint16 port;

		EventEngine();
	public:
		~EventEngine();

		/**
		* Equivalent to calling SDL_PollEvent()
		*/
		void pollEvents();
		
		bool isPressed(Key);
		bool isPressed(Mouse);
    
        /**
         * Software emulation of keypresses
         */
        void setPressed(Key);
        void setPressed(Mouse);
	
		void setMouseRelative(bool);

		/**
		* Returns mouse's delta position
		* It's the difference between current and
		* previous mouse positions
		*
		*/
		Point2 getMouseDPos();

		/**
		* Starts executing android "joystick" listener
		* on a separate thread
		*/
		void runConnThread();
};

#endif

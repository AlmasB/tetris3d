#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include <string>
#include <thread>

#include <SDL.h>
#include <SDL_net.h>

#include "EngineCommon.h"
#include "GameMath.h"



//just the useful ones for now
enum Key {
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, LAST
};

enum Mouse {
	BTN_LEFT, BTN_RIGHT, BTN_LAST
};

class EventEngine {
	friend class GameEngine;
	private:
		bool running;
		SDL_Event event;
		bool keys[Key::LAST];
		bool buttons[Mouse::BTN_LAST];
		Point2 mouseDPos;

		void updateKeys(const SDL_Keycode &, bool);


		bool androidCtrlEnabled;
		std::thread * connThread;

		IPaddress ip, *remoteip;
		TCPsocket server, client;
		char message[1024];	// we're only expecting commands, make buf smaller
		int len;
		Uint32 ipaddr;
		Uint16 port;

		EventEngine();
	public:
		~EventEngine();

		std::string init();

		bool isRunning();

		/**
		* Equivalent to calling SDL_PollEvent()
		*/
		void pollEvents();
		
		bool isPressed(Key);
		bool isPressed(Mouse);
		
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
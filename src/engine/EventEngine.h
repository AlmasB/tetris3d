#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include <string>
#include <thread>

#include <SDL.h>
#include <SDL_net.h>

#include "GameMath.h"
#include "Debug.h"

#define _ENGINE_ERROR_NET_RESOLVE_HOST "Failed to resolve host"
#define _ENGINE_ERROR_NET_OPEN "Failed to open socket"

//just the useful ones for now
enum Key {
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, LAST
};

enum Mouse {
	BTN_LEFT, BTN_RIGHT, BTN_LAST
};

class EventEngine {
	private:
		bool running;
		SDL_Event event;
		//bool keys[Key::LAST];
		bool buttons[Mouse::BTN_LAST];
		Point2 mouseDPos;

		void updateKeys(const SDL_Keycode &, bool);

		///////////////////////////////// TEST////////////////////

		


	public:
		EventEngine();
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

		//////////////////////////////////////////////////////
		// hide data and use public function for thread run
		bool keys[Key::LAST];
		std::thread * connThread;

		IPaddress ip, *remoteip;
		TCPsocket server, client;
		char message[1024];	// we're only expecting commands, make buf smaller
		int len;
		Uint32 ipaddr;
		Uint16 port;

		void runConnThread();
};

#endif
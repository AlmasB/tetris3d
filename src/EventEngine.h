#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include "Common.h"

#include <SDL.h>

//just the useful ones for now
enum Key {
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, LAST
};

enum Mouse {
	BTN_LEFT, BTN_RIGHT, BTN_LAST
};

class EventEngine {
	private:
		SDL_Event event;
		bool keys[Key::LAST];
		bool buttons[Mouse::BTN_LAST];
		Point2 mouseDPos;

		void updateKeys(const SDL_Keycode &, bool);
	public:
		EventEngine();

		/**
		* Equivalent to calling SDL_PollEvent()
		* Updates only key states in the meantime
		*/
		void pollEvents();
		
		bool isPressed(Key);
		bool isPressed(Mouse);

		bool mouseClicked(Mouse);
		
		/**
		* Returns mouse's delta position
		* It's the difference between current and
		* previous mouse positions
		*
		*/
		Point2 getMouseDPos();
};

#endif
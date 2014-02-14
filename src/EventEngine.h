#ifndef __EVENT_ENGINE_H__
#define __EVENT_ENGINE_H__

#include "Common.h"

#include <SDL.h>

//just the useful ones for now
enum Key {
	W, S, A, D, ESC, SPACE, UP, DOWN, LEFT, RIGHT, LAST
};

class EventEngine {
	private:
		SDL_Event event;
		bool keys[Key::LAST];

		void updateKeys(const SDL_Keycode &, bool);
	public:
		EventEngine();
		void pollEvents();
		
		bool isPressed(Key);

};

#endif
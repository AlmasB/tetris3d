#include "EventEngine.h"

EventEngine::EventEngine() : running(true) {
	for (uint i = 0; i < Key::LAST; ++i) {
		keys[i] = false;
	}

	buttons[Mouse::BTN_LEFT] = false;
	buttons[Mouse::BTN_RIGHT] = false;
}

EventEngine::~EventEngine() {
#ifdef __DEBUG
	debug("EventEngine::~EventEngine() started");
#endif

	running = false;
	connThread->join();	// wait till it finishes

	if (nullptr != client)
		SDLNet_TCP_Close(client);
	//SDLNet_TCP_Close(server);

	delete connThread;
#ifdef __DEBUG
	debug("EventEngine::~EventEngine() finished");
#endif
}

std::string EventEngine::init() {
	port = (Uint16)strtol("55555", NULL, 0);	// replace with int value
	if (-1 == SDLNet_ResolveHost(&ip, NULL, port))
		return "Failed to resolve host at port: " + std::to_string(port) + std::string(SDLNet_GetError());

	server = SDLNet_TCP_Open(&ip);
	if (nullptr == server)
		return "Failed to create local server " + std::string(SDLNet_GetError());

	connThread = new std::thread(&EventEngine::runConnThread, this);

	return "";	// same as _ENGINE_ERROR_NONE
}

void EventEngine::runConnThread() {
#ifdef __DEBUG
	debug("EventEngine::runConnThread() started on a separate thread");
#endif

	while (running) {
		if (nullptr == client) {
			client = SDLNet_TCP_Accept(server);
		}
		else {
			remoteip = SDLNet_TCP_GetPeerAddress(client);	// TODO: do init once and redo on connection lost
			ipaddr = SDL_SwapBE32(remoteip->host);
			len = SDLNet_TCP_Recv(client, message, 1024);

			for (int i = 0; i < len; ++i) {
				char c = (int)message[i];

				if ('W' == c && !keys[Key::W]) {	// TODO: we don't really want that, do smth smoother
					keys[Key::W] = true;
				}
				else {
					keys[Key::W] = false;
				}

				if ('S' == c && !keys[Key::S]) {
					keys[Key::S] = true;
				}
				else {
					keys[Key::S] = false;
				}

				if ('A' == c && !keys[Key::A]) {
					keys[Key::A] = true;
				}
				else {
					keys[Key::A] = false;
				}

				if ('D' == c && !keys[Key::D]) {
					keys[Key::D] = true;
				}
				else {
					keys[Key::D] = false;
				}


			}
		}
		SDL_Delay(100);
	}

#ifdef __DEBUG
	debug("EventEngine::runConnThread() finished on a separate thread");
#endif
}

bool EventEngine::isRunning() {
	return running;
}

void EventEngine::pollEvents() {
	while (SDL_PollEvent(&event)) {
		if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
			updateKeys(event.key.keysym.sym, event.type == SDL_KEYDOWN);
		}

		buttons[Mouse::BTN_LEFT] = !((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) == 0);
		buttons[Mouse::BTN_RIGHT] = !((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) == 0);
	}


}

void EventEngine::updateKeys(const SDL_Keycode &key, bool keyDown) {
	Key index;

	switch (key) {
		case SDLK_RIGHT:	index = Key::RIGHT; break;
		case SDLK_d:		index = Key::D; break;
		case SDLK_LEFT:		index = Key::LEFT; break; 
		case SDLK_a:		index = Key::A; break;
		case SDLK_UP:		index = Key::UP; break;
		case SDLK_w:		index = Key::W; break;
		case SDLK_DOWN:		index = Key::DOWN; break;
		case SDLK_s:		index = Key::S; break;
		case SDLK_ESCAPE:	index = Key::ESC; break;
		case SDLK_SPACE:	index = Key::SPACE; break;
		default:
			return;	// we don't care about other keys, at least now
	}

	keys[index] = keyDown;
}

bool EventEngine::isPressed(Key key) {
	return keys[key];
}

bool EventEngine::isPressed(Mouse btn) {
	return buttons[btn];
}

Point2 EventEngine::getMouseDPos() {
	SDL_GetRelativeMouseState(&mouseDPos.x, &mouseDPos.y);
	return mouseDPos;
}
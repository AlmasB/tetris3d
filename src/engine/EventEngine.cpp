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
		return _ENGINE_ERROR_NET_RESOLVE_HOST + std::string(SDLNet_GetError());

	server = SDLNet_TCP_Open(&ip);
	if (nullptr == server)
		return _ENGINE_ERROR_NET_OPEN + std::string(SDLNet_GetError());

	connThread = new std::thread(runConnThread, this);

	return "";
}

bool EventEngine::isRunning() {
	return running;
}

void runConnThread(EventEngine * engine) {
#ifdef __DEBUG
	debug("EventEngine&&runConnThread() started");
#endif

	while (engine->isRunning()) {
		if (nullptr == engine->client) {
			engine->client = SDLNet_TCP_Accept(engine->server);
		}
		if (nullptr == engine->client) {
			//std::cout << "error" << std::endl;
		}
		else {
			engine->remoteip = SDLNet_TCP_GetPeerAddress(engine->client);
			engine->ipaddr = SDL_SwapBE32(engine->remoteip->host);
			engine->len = SDLNet_TCP_Recv(engine->client, engine->message, 1024);

			for (int i = 0; i < engine->len; ++i) {
				char c = (int)engine->message[i];

				if ('W' == c && !engine->keys[Key::W]) {
					engine->keys[Key::W] = true;
				}
				else {
					engine->keys[Key::W] = false;
				}

				if ('S' == c && !engine->keys[Key::S]) {
					engine->keys[Key::S] = true;
				}
				else {
					engine->keys[Key::S] = false;
				}

				if ('A' == c && !engine->keys[Key::A]) {
					engine->keys[Key::A] = true;
				}
				else {
					engine->keys[Key::A] = false;
				}

				if ('D' == c && !engine->keys[Key::D]) {
					engine->keys[Key::D] = true;
				}
				else {
					engine->keys[Key::D] = false;
				}


				/*std::cout << c;

				if (i == engine->len - 1)
					std::cout << std::endl;*/
			}
		}
		SDL_Delay(100);
	}
#ifdef __DEBUG
	debug("EventEngine&&runConnThread() finished");
#endif
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
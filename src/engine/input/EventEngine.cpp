#include "EventEngine.h"

EventEngine::EventEngine() : running(true), androidCtrlEnabled(false) {
	for (int i = 0; i < Key::LAST; ++i) {
		keys[i] = false;
	}

	buttons[Mouse::BTN_LEFT] = false;
	buttons[Mouse::BTN_RIGHT] = false;

	if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {	// trap mouse inside for fps mode
#ifdef __DEBUG
		debug("Warning: SDL_SetRelativeMouseMode() isn't supported");
#endif
	}

	if (SDLNet_Init() < 0)
		throw EngineException("Failed to init SDL_net", SDLNet_GetError());

	if (androidCtrlEnabled) {

		port = (Uint16)strtol("55555", NULL, 0);	// TODO: replace with int value
		if (SDLNet_ResolveHost(&ip, NULL, port) < 0)	// TODO: if we can't use android as controller 
			throw EngineException("Failed to resolve host at port 55555", SDLNet_GetError());	// no reason to throw exceptions to leave the game

		server = SDLNet_TCP_Open(&ip);
		if (nullptr == server)
			throw EngineException("Failed to create local server", SDLNet_GetError());

		connThread = new std::thread(&EventEngine::runConnThread, this);
	}
}

EventEngine::~EventEngine() {
#ifdef __DEBUG
	debug("EventEngine::~EventEngine() started");
#endif

	running = false;

	if (connThread)
		connThread->join();	// wait till it finishes

	if (nullptr != client)
		SDLNet_TCP_Close(client);
	//SDLNet_TCP_Close(server);

	safeDelete(connThread);
	SDLNet_Quit();

#ifdef __DEBUG
	debug("EventEngine::~EventEngine() finished");
#endif
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
			len = SDLNet_TCP_Recv(client, message, 1024);	// doesn't return until catches a tcp packet

#ifdef __DEBUG
			std::cout << "Received: " << len << " bytes" << std::endl;
#endif

			for (int i = 0; i < len; ++i) {
				char c = (int)message[i];
			
				keys[Key::W] = ('W' == c && !keys[Key::W]);
				keys[Key::S] = ('S' == c && !keys[Key::S]);
				keys[Key::A] = ('A' == c && !keys[Key::A]);
				keys[Key::D] = ('D' == c && !keys[Key::D]);
				keys[Key::LEFT]  = ('L' == c && !keys[Key::LEFT]);
				keys[Key::RIGHT] = ('R' == c && !keys[Key::RIGHT]);


				if ('N' == c) {
					for (int i = 0; i < Key::LAST; ++i)
						keys[i] = false;
				}

				std::cout << c << std::endl;
			}
		}
		SDL_Delay(16);	// TODO: too specific, design flaw, need to isolate the engine side
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
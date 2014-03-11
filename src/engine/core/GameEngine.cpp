#include "GameEngine.h"

std::shared_ptr<GameEngine> GameEngine::instance = nullptr;

GameEngine::GameEngine() {
	std::cout << "Initializing The Cube Game Engine v." << __ENGINE_VERSION_MAJOR
		<< "." << __ENGINE_VERSION_MINOR << std::endl;

#ifdef __DEBUG
	#if defined(_WIN32)
		debug("WIN32");
	#elif defined(__linux__)
		debug("LINUX");
	#endif
#endif

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		throw EngineException("SDL_Init()", SDL_GetError());

#ifdef __DEBUG
	debug("SDL_Init() successful");
#endif

	Uint32 ticks = SDL_GetTicks();
	srand(ticks);	// init random seed

#ifdef __DEBUG
	debug("Inited srand() with", ticks);
#endif

	try {
		gfxInstance = std::shared_ptr<GraphicsEngine>(new GraphicsEngine());

#ifdef __DEBUG
		debug("GraphicsEngine() successful");
#endif

		audioInstance = std::shared_ptr<AudioEngine>(new AudioEngine());

#ifdef __DEBUG
		debug("AudioEngine() successful");
#endif

		eventInstance = std::shared_ptr<EventEngine>(new EventEngine());

#ifdef __DEBUG
		debug("EventEngine() successful");
#endif
	}
	catch (EngineException & e) {
		throw e;
	}
}

GameEngine::~GameEngine() {
#ifdef __DEBUG
	debug("GameEngine::~GameEngine() started");
#endif

	ResourceManager::freeResources();

	eventInstance.reset();
	gfxInstance.reset();

#ifdef __DEBUG
	debug("GameEngine::~GameEngine() finished");
#endif
}

void GameEngine::quit() {
	if (instance)
		instance.reset();
}

std::shared_ptr<GameEngine> GameEngine::getInstance() {
	if (!instance)
		instance = std::shared_ptr<GameEngine>(new GameEngine());
	return instance;
}
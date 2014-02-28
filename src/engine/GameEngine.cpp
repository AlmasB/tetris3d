#include "GameEngine.h"

std::shared_ptr<GameEngine> GameEngine::instance = nullptr;

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

bool GameEngine::init(std::vector<std::string> resources) {
	std::cout << "Initializing the game engine v." << __ENGINE_VERSION_MAJOR 
		<< "." << __ENGINE_VERSION_MINOR << std::endl;

#ifdef __DEBUG
	#if defined(_WIN32)
		debug("WIN32");
	#elif defined(__linux__)
		debug("LINUX");
	#endif
#endif

	std::string error;
	gfxInstance = std::shared_ptr<GraphicsEngine>(new GraphicsEngine());
	if ((error = gfxInstance->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		return false;
	}

#ifdef __DEBUG
	debug("GraphicsEngine::init() successful");
#endif

	eventInstance = std::shared_ptr<EventEngine>(new EventEngine());
	if ((error = eventInstance->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		return false;
	}

#ifdef __DEBUG
	debug("EventEngine::init() successful");
#endif

	if ((error = ResourceManager::loadResources(resources)) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		return false;
	}

#ifdef __DEBUG
	debug("ResourceManager::loadResources() successful");
#endif

	return _ENGINE_ERROR_NONE;
}

std::shared_ptr<GraphicsEngine> GameEngine::getGraphicsEngine() {
	return gfxInstance;
}

std::shared_ptr<EventEngine> GameEngine::getEventEngine() {
	return eventInstance;
}
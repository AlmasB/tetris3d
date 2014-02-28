#include "GameEngine.h"

std::shared_ptr<GameEngine> GameEngine::instance = nullptr;

std::shared_ptr<GameEngine> GameEngine::getInstance() {
	if (!instance)
		instance = std::shared_ptr<GameEngine>(new GameEngine());
	return instance;
}

bool GameEngine::init(std::vector<std::string> resources) {
	std::cout << "Initializing the game engine v." << __ENGINE_VERSION_MAJOR 
		<< "." << __ENGINE_VERSION_MINOR << std::endl;

	std::string error;
	gfxInstance = std::shared_ptr<GraphicsEngine>(new GraphicsEngine());
	if ((error = gfxInstance->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	eventInstance = std::shared_ptr<EventEngine>(new EventEngine());
	if ((error = eventInstance->init()) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}

	if ((error = ResourceManager::loadResources(resources)) != _ENGINE_ERROR_NONE) {
		std::cout << error << std::endl;
		getchar();
		return false;
	}


	return _ENGINE_ERROR_NONE;
}

std::shared_ptr<GraphicsEngine> GameEngine::getGraphicsEngine() {
	return gfxInstance;
}

std::shared_ptr<EventEngine> GameEngine::getEventEngine() {
	return eventInstance;
}
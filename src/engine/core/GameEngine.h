#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

#include <string>
#include <vector>
#include <memory>

#include "../graphics/GraphicsEngine.h"
#include "../input/EventEngine.h"
#include "ResourceManager.h"
#include "Timer.h"

#define __ENGINE_VERSION_MAJOR 0
#define __ENGINE_VERSION_MINOR 1

class GameEngine {
	private:
		static std::shared_ptr<GameEngine> instance;
		std::shared_ptr<GraphicsEngine> gfxInstance;
		std::shared_ptr<EventEngine> eventInstance;
	public:
		static std::shared_ptr<GameEngine> getInstance();
		~GameEngine();
		static void quit();

		/**
		* After acquiring an instance of game engine
		* you have explicitly call this function to initialise
		* the engine and all other subsystems
		*
		* @return true if all subsystems inited correctly, false otherwise
		* @post-condition it will be safe to use any lib related calls
		*/
		bool init(std::vector<std::string> resources);
		std::shared_ptr<GraphicsEngine> getGraphicsEngine() { return gfxInstance; }
		std::shared_ptr<EventEngine> getEventEngine() {	return eventInstance; }
};


#endif
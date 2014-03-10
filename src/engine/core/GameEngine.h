#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

#include <string>
#include <vector>
#include <memory>

#include "../graphics/GraphicsEngine.h"
#include "../audio/AudioEngine.h"
#include "../input/EventEngine.h"
#include "ResourceManager.h"
#include "Timer.h"

#define __ENGINE_VERSION_MAJOR 0
#define __ENGINE_VERSION_MINOR 1

class GameEngine {
	private:
		static std::shared_ptr<GameEngine> instance;
		std::shared_ptr<GraphicsEngine> gfxInstance;
		std::shared_ptr<AudioEngine> audioInstance;
		std::shared_ptr<EventEngine> eventInstance;

		GameEngine();
	public:
		/**
		* @return the instance of game engine
		* @exception throws EngineException if init of any submodules failed
		*/
		static std::shared_ptr<GameEngine> getInstance();
		~GameEngine();
		static void quit();

		std::shared_ptr<GraphicsEngine> getGraphicsEngine() { return gfxInstance; }
		std::shared_ptr<AudioEngine> getAudioEngine() { return audioInstance; }
		std::shared_ptr<EventEngine> getEventEngine() {	return eventInstance; }
};


#endif
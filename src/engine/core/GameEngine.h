/**
 * {XCube3D Game Engine}
 * Copyright(C) { 2013-2014 }  {AlmasB} {a.baimagambetov1@uni.brighton.ac.uk}
 *
 * This program is free software; you can redistribute it and / or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.
 */

#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__

#include <string>
#include <vector>
#include <memory>

#include "../graphics/GraphicsEngine.h"
#include "../audio/AudioEngine.h"
#include "../input/EventEngine.h"
#include "../physics/PhysicsEngine.h"
#include "ResourceManager.h"
#include "Timer.h"

#define _ENGINE_VERSION_MAJOR 0
#define _ENGINE_VERSION_MINOR 2

class GameEngine {
	private:
		static std::shared_ptr<GameEngine> instance;
		std::shared_ptr<GraphicsEngine> gfxInstance;
		std::shared_ptr<AudioEngine> audioInstance;
		std::shared_ptr<EventEngine> eventInstance;
		std::shared_ptr<PhysicsEngine> physicsInstance;

		GameEngine();
	public:
		/**
		 * @return the instance of game engine
		 * @exception throws EngineException if init of any submodules failed
		 */
		static std::shared_ptr<GameEngine> getInstance();
		~GameEngine();
        
		/**
		 * Quits the engine, closes all the subsystems
		 * 
		 * All subsequent calls to any of subsystems will have undefined behaviour
		 */
		static void quit();
		
		/**
		 * Subsystems can only be accessed via the following accessors  
		 * @return approriate subsystem of the engine
		 */
		std::shared_ptr<GraphicsEngine> getGraphicsEngine() { return gfxInstance; }
		std::shared_ptr<AudioEngine> getAudioEngine() { return audioInstance; }
		std::shared_ptr<EventEngine> getEventEngine() {	return eventInstance; }
		std::shared_ptr<PhysicsEngine> getPhysicsEngine() { return physicsInstance; }
};

#endif

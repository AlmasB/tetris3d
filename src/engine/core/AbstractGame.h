#ifndef __ABSTRACT_GAME_H__
#define __ABSTRACT_GAME_H__

#include "GameEngine.h"

class AbstractGame {
	private:
		void handleMouseEvents();
		void updatePhysics();

	protected:
		AbstractGame();
		virtual ~AbstractGame();

		/* ENGINE OBJECTS */
		std::shared_ptr<GraphicsEngine> gfx;
		std::shared_ptr<AudioEngine> sfx;
		std::shared_ptr<EventEngine> eventSystem;
		std::shared_ptr<PhysicsEngine> physics;
		std::shared_ptr<Camera> camera;

		/* Main loop control */
		bool running;
		double gameTime;

		virtual void handleKeyEvents() = 0;

		virtual void onLeftMouseButton();
		virtual void onRightMouseButton();

		virtual void update() = 0;
		virtual void render() = 0;

		virtual void renderUI();
	public:
		int runMainLoop();
};

#endif
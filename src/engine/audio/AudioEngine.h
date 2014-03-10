#ifndef __AUDIO_ENGINE_H__
#define __AUDIO_ENGINE_H__

#include <SDL_mixer.h>

#include "../core/EngineCommon.h"

class AudioEngine {
	friend class GameEngine;
	private:
		AudioEngine();
		bool soundOn;
	public:
		~AudioEngine();
		void toggleSound();
		void playSound(Mix_Chunk * sound);
};

#endif
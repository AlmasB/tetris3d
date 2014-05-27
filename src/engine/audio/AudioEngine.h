#ifndef __AUDIO_ENGINE_H__
#define __AUDIO_ENGINE_H__

#include <SDL2/SDL_mixer.h>

#include "../core/EngineCommon.h"

class AudioEngine {
	friend class GameEngine;
	private:
		AudioEngine();
		bool soundOn;
		int volume;
	public:
		~AudioEngine();
		void toggleSound();

		/**
		* Controls the volume of all sounds
		* @param volume - in the range [0..128]
		*/
		void setSoundVolume(const int &);
		int getSoundVolume();

		void playSound(Mix_Chunk * sound);

		/**
		* Call this to manually specify the volume of the sound
		*
		* @param sound - the sound to play
		* @param volume - the volume at which to play in range [0..128]
		*/
		void playSound(Mix_Chunk * sound, const int & _volume);
};

#endif
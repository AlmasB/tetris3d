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

		/**
		* Plays mp3 file given amount of times
		*
		* @param mp3 - the file to play
		* @param times - number of times, -1 will play indefinitely
		*/
		void playMP3(Mix_Music * mp3, const int & times);
};

#endif
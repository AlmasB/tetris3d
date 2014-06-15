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

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <vector>

#include "../graphics/GraphicsEngine.h"
#include "../audio/AudioEngine.h"

class ResourceManager {
	private:
		static std::map<std::string, GLuint> textures;
		static std::map<std::string, Mix_Chunk *> sounds;
		static std::map<std::string, Mix_Music *> mp3files;
		static std::map<std::string, TTF_Font *> fonts;
	public:
		/**
		 * Loads all resources into several maps (cache)
		 * to make the program run smoother
		 *
		 * @param fileNames - vector containing names to resource files
		 *
		 * The resources can be retrieved with an appropriate call to get*
		 */
		static void loadResources(std::vector<std::string> fileNames);

		/**
		* Call to free all resource files
		* All subsequent calls to get* will return nullptr or cause undefined behaviour
		*/
		static void freeResources();

		static GLuint getTextureID(std::string fileName);
		static TTF_Font * getFont(std::string fileName);
		static Mix_Chunk * getSound(std::string fileName);
		static Mix_Music * getMP3(std::string fileName);
};

#endif

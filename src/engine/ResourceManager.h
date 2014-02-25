#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>

#include <SDL_image.h>
#include <GL/glew.h>

#include "Debug.h"

class ResourceManager {
	private:
		static std::map<std::string, GLuint> textures;

		static GLuint loadTexture(const char * fileName);
	public:
		static std::string loadResources();

		/* GLuint is typedef unsigned int, 
		so no reason to pull GL/glew just for that*/
		static GLuint getTextureID(std::string fileName);
};

#endif

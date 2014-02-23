#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>

#include <SDL_image.h>
#include <GL/glew.h>

class ResourceManager {
	private:
		static std::map<const char *, GLuint> textures;

		static GLuint loadTexture(const char * fileName);
	public:
		static void loadResources();

		/* GLuint is typedef unsigned int, 
		so no reason to pull GL/glew just for that*/
		static GLuint getTextureID(const char * fileName);
};

#endif
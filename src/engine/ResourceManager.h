#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <vector>

#include "GraphicsEngine.h"

class ResourceManager {
	private:
		static std::map<std::string, GLuint> textures;
	public:
		static std::string loadResources(std::vector<std::string> fileNames);

		/* GLuint is typedef unsigned int, 
		so no reason to pull GL/glew just for that*/
		static GLuint getTextureID(std::string fileName);
};

#endif

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <vector>

#include "GraphicsEngine.h"

class ResourceManager {
	private:
		static std::map<std::string, GLuint> textures;
		static std::map<std::string, TTF_Font *> fonts;
	public:
		/**
		* Loads all resources into several maps (cache)
		*
		* The resources can be retrieved with an appropriate call to get*
		*/
		static std::string loadResources(std::vector<std::string> fileNames);
		static void freeResources();


		static GLuint getTextureID(std::string fileName);
		static TTF_Font * getFont(std::string fileName);
};

#endif

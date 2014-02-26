#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::textures;

std::string ResourceManager::loadResources(std::vector<std::string> fileNames) {
	for (auto file : fileNames) {
		SDL_Surface * surf = IMG_Load(file.c_str());
		if (nullptr == surf)
			return _ENGINE_ERROR_RESOURCE + std::string(IMG_GetError());

		textures[file] = GFX::createGLTextureFromSurface(surf);
		SDL_FreeSurface(surf);
		if (0 == textures[file])
			return _ENGINE_ERROR_RESOURCE + file;
	}

	return _ENGINE_ERROR_NONE;
}

GLuint ResourceManager::getTextureID(std::string fileName) {
	return textures[fileName];
}

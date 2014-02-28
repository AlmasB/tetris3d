#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::textures;
std::map<std::string, TTF_Font *> ResourceManager::fonts;

/**
* @return true if param "str" contains param "sequence" chars, false otherwise 
*/
bool stringContains(const std::string & str, const char * sequence) {
	return -1 != str.find(sequence);
}

std::string ResourceManager::loadResources(std::vector<std::string> fileNames) {
	for (auto file : fileNames) {
		if (stringContains(file, ".ttf")) {
			TTF_Font * font = TTF_OpenFont(file.c_str(), 36);	// hardcode font size ?
			if (nullptr == font)
				return "Failed to load font: " + file + std::string(TTF_GetError());
			fonts[file] = font;
		}
		else if (stringContains(file, ".png")) {
			SDL_Surface * surf = IMG_Load(file.c_str());
			if (nullptr == surf)
				return "Failed to load image: " + file + " " + std::string(IMG_GetError());

			textures[file] = GFX::createGLTextureFromSurface(surf);
			SDL_FreeSurface(surf);
			if (0 == textures[file])
				return "Failed to create GL Texture from surface file: " + file;
		}
	}

	return _ENGINE_ERROR_NONE;
}

void ResourceManager::freeResources() {
	for (auto pair : fonts)
		TTF_CloseFont(pair.second);
}

GLuint ResourceManager::getTextureID(std::string fileName) {
	return textures[fileName];
}

TTF_Font * ResourceManager::getFont(std::string fileName) {
	return fonts[fileName];
}

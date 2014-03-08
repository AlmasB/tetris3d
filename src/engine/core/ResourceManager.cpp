#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::textures;
std::map<std::string, TTF_Font *> ResourceManager::fonts;
std::map<std::string, Mix_Chunk *> ResourceManager::sounds;

std::string ResourceManager::loadResources(std::vector<std::string> fileNames) {
	for (auto file : fileNames) {
		if (file.find(".ttf") != -1) {
			TTF_Font * font = TTF_OpenFont(file.c_str(), 36);	// hardcode font size ?
			if (nullptr == font)
				return "Failed to load font: " + file + std::string(TTF_GetError());
			fonts[file] = font;
		}
		else if (file.find("wav") != -1) {
			Mix_Chunk * sound = Mix_LoadWAV(file.c_str());
			if (nullptr == sound)
				return "Failed to load sound: " + file + " " + std::string(Mix_GetError());
			sounds[file] = sound;
		}
		else if (file.find(".png") != -1) {
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
	for (auto pair : fonts) {
		if (pair.second) {
			TTF_CloseFont(pair.second);
#ifdef __DEBUG
			debug("Font freed:");
			debug(pair.first.c_str());
#endif
		}
	}

	for (auto pair : textures) {
		if (pair.second) {
			glDeleteTextures(1, &pair.second);
		}
	}

#ifdef __DEBUG
	debug("Textures deleted");
#endif

	for (auto pair : sounds) {
		if (pair.second) {
			Mix_FreeChunk(pair.second);
#ifdef __DEBUG
			debug("Sound freed:");
			debug(pair.first.c_str());
#endif
		}
	}

#ifdef __DEBUG
	debug("ResourceManager::freeResources() finished");
#endif
}

GLuint ResourceManager::getTextureID(std::string fileName) {
	return textures[fileName];
}

TTF_Font * ResourceManager::getFont(std::string fileName) {
	return fonts[fileName];
}

Mix_Chunk * ResourceManager::getSound(std::string fileName) {
	return sounds[fileName];
}

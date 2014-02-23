#include "ResourceManager.h"

std::map<const char *, GLuint> ResourceManager::textures;

void ResourceManager::loadResources() {
	// implement
	// load resource file names as a list/vector
	// TODO: automate
	textures["brick.png"] = loadTexture("brick.png");
	textures["prize.png"] = loadTexture("prize.png");
}

GLuint ResourceManager::getTextureID(const char * fileName) {
	return textures[fileName];
}

GLuint ResourceManager::loadTexture(const char * fileName) {
	GLuint texture;

	SDL_Surface* surface = IMG_Load(fileName);
	if (surface == nullptr) {
		// if debug show error message here SDL_GetError()
		return 0;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	//glBindTexture(GL_TEXTURE_2D, 0);	// unbind

	return texture;
}
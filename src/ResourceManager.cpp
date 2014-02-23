#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::textures;

std::string ResourceManager::loadResources() {
	// implement
	// load resource file names as a list/vector
	// TODO: automate
	textures["res/brick.png"] = loadTexture("res/brick.png");
	textures["res/prize.png"] = loadTexture("res/prize.png");

	return "";
}

GLuint ResourceManager::getTextureID(std::string fileName) {
	return textures[fileName];
}

GLuint ResourceManager::loadTexture(const char * fileName) {
	GLuint texture;

	SDL_Surface* surface = IMG_Load(fileName);
	if (surface == nullptr) {
		// if debug show error message here SDL_GetError() maybe?
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

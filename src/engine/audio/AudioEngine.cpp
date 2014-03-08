#include "AudioEngine.h"

AudioEngine::AudioEngine() : soundOn(true) {}

AudioEngine::~AudioEngine() {

}

std::string AudioEngine::init() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
		return "Failed to init SDL_mixer: " + std::string(Mix_GetError());

	return _ENGINE_ERROR_NONE;
}

void AudioEngine::toggleSound() {
	soundOn = !soundOn;
}

void AudioEngine::playSound(Mix_Chunk * sound) {
	if (soundOn)
		Mix_PlayChannel(-1, sound, 0);
}
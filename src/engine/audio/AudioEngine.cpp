#include "AudioEngine.h"

AudioEngine::AudioEngine() : soundOn(true) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) < 0)
		throw EngineException("Failed to init SDL_mixer:", Mix_GetError());
}

AudioEngine::~AudioEngine() {}

void AudioEngine::toggleSound() {
	soundOn = !soundOn;
}

void AudioEngine::playSound(Mix_Chunk * sound) {
	if (soundOn)
		Mix_PlayChannel(-1, sound, 0);
}
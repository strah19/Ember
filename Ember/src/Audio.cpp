#include "Audio.h"

namespace Ember {
	AudioChunk::AudioChunk(const std::string& file_path) {
		Initialize(file_path);
	}

	void AudioChunk::Initialize(const std::string& file_path) {
		chunk = Mix_LoadWAV(file_path.c_str());
		volume = 0;
	}

	void AudioChunk::Play() {
		Mix_PlayChannel(-1, chunk, 0);
	}

	void AudioChunk::Volume(unsigned int volume) {
		volume = (volume < 128) ? volume : 128;
		Mix_VolumeChunk(chunk, volume);
	}

	void AudioChunk::Disable() {
		Mix_HaltChannel(-1);
	}

	void AudioChunk::Resume() {
		Mix_Resume(-1);
	}

	void AudioChunk::Pause() {
		Mix_Pause(-1);
	}

	AudioChunk::~AudioChunk() {
		Mix_FreeChunk(chunk);
	}

	AudioMusic::AudioMusic(const std::string& file_path) {
		Initialize(file_path);
	}

	void AudioMusic::Initialize(const std::string& file_path) {
		music = Mix_LoadMUS(file_path.c_str());
		volume = 0;
	}

	void AudioMusic::Play() {
		Mix_PlayMusic(music, -1);
	}

	void AudioMusic::Disable() {
		Mix_HaltMusic();
	}

	void AudioMusic::Resume() {
		Mix_ResumeMusic();
	}

	void AudioMusic::Pause() {
		Mix_PauseMusic();
	}

	void AudioMusic::Volume(unsigned int volume) {
		volume = (volume < 128) ? volume : 128;
		Mix_VolumeMusic(volume);
	}

	AudioMusic::~AudioMusic() {
		Mix_FreeMusic(music);
	}
}
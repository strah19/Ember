#ifndef AUDIO_H
#define AUDIO_H

#include "Ember.h"

namespace Ember {
	class AudioChunk {
	public:
		AudioChunk(const std::string& file_path);
		AudioChunk() : chunk(nullptr), volume(0) { }
		void Initialize(const std::string& file_path);

		void Play();
		void Volume(unsigned int volume);

		void Pause();
		void Resume();
		void Disable();
		virtual ~AudioChunk();
	private:
		Mix_Chunk* chunk;
		unsigned int volume;
	};

	class AudioMusic {
	public:
		AudioMusic(const std::string& file_path);
		AudioMusic() : music(nullptr), volume(0) { }
		void Initialize(const std::string& file_path);

		void Play();
		void Volume(unsigned int volume);

		void Pause();
		void Resume();
		void Disable();
		virtual ~AudioMusic();
	private:
		Mix_Music* music;
		unsigned int volume;
	};
}

#endif // !AUDIO_H

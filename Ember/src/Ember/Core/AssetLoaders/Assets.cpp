#include "EmberPch.h"
#include "Assets.h"

namespace ember {
	static std::vector<TTF_Font*>fonts;

	Font::Font(std::shared_ptr<Renderer2D> renderer, const char* file_path, const char* text, int size, const Color& color, int x, int y)
		: renderer(renderer), font_width(0), font_height(0), font(TTF_OpenFont(file_path, size)), font_position({ x, y, 0, 0 }),
		font_texture(NULL), font_color(color), font_is_locked(true) {
		fonts.push_back(font);
		if(font) {
			UnlockFont();
			ChangeFont(text, color);
		}
	}

	void Font::UnlockFont() {
		font_is_locked = false;
	}

	void Font::LockFont() {
		font_is_locked = true;
	}

	void Font::Render() {
		if (!font_is_locked) {
			SDL_RenderCopy(renderer->Renderer(), font_texture, NULL, &font_position);
		}
	}

	void Font::SetPosition(int x, int y) {
		if (!font_is_locked) {
			font_position.x = x;
			font_position.y = y;
		}
	}

	void Font::Update(int x, int y) {
		if (!font_is_locked) {
			font_position.x = font_position.x + x;
			font_position.y = font_position.y + y;
		}
	}

	AudioChunk::AudioChunk(const char* file_path)
		: m_chunk(Mix_LoadWAV(file_path)), m_volume(0) {
	}

	void AudioChunk::Play() {
		Mix_PlayChannel(-1, m_chunk, 0);
	}

	void AudioChunk::Volume(unsigned int volume) {
		m_volume = (volume < 128) ? volume : 128;
		Mix_VolumeChunk(m_chunk, m_volume);
	}

	AudioChunk::~AudioChunk() {
		Mix_FreeChunk(m_chunk);
	}

	AudioMusic::AudioMusic(const char* file_path)
		: m_music(Mix_LoadMUS(file_path)), m_volume(0) {
	}

	void AudioMusic::Play() {
		Mix_PlayMusic(m_music, -1);
	}

	void AudioMusic::Volume(unsigned int volume) {
		m_volume = (volume < 128) ? volume : 128;
		Mix_VolumeMusic(m_volume);
	}

	AudioMusic::~AudioMusic() {
		Mix_FreeMusic(m_music);
	}
}
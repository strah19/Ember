#include "AssetLoader.h"
#include <iostream>

namespace Ember
{
	Font::Font(const std::string& filePath, const std::string& text, int size, const SDL_Color& c)
		: FontID(0), FontWidth(0), FontHeight(0), FontFilePath(filePath), 
		FontData(TTF_OpenFont(filePath.c_str(), size)), FontPosition({ 0, 0, 0, 0 }), 
		FontTexture(NULL), FontColor(c), FontIsLocked(false), Created(false)
	{
		if (!FontData) {
			SDL_Quit();
			std::cout << "Could Not Load Font Data From: " << filePath << std::endl;
		}
		else {
			Created = true;
			std::cout << "Loaded Font Data From: " << filePath << std::endl;
		}
	}

	void Font::unlock_font()
	{
		FontIsLocked = false;
	}

	void Font::change_font(const std::string& Text, const SDL_Color& color, SDL_Renderer* renderer)
	{
		if (!FontIsLocked) {
			FontColor = color;
			SDL_DestroyTexture(FontTexture);
			FontText = Text;
			SDL_Surface* fontSurface = TTF_RenderText_Blended(FontData, Text.c_str(), FontColor);
			FontWidth = fontSurface->w;
			FontHeight = fontSurface->h;

			FontPosition.w = FontWidth;
			FontPosition.h = FontHeight;

			FontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);

			SDL_FreeSurface(fontSurface);
		}
	}

	void Font::lock_font()
	{
		FontIsLocked = true;
	}

	void Font::render(SDL_Renderer* renderer)
	{
		if (!FontIsLocked)
			SDL_RenderCopy(renderer, FontTexture, NULL, &FontPosition);
	}

	void Font::set_position(int x, int y)
	{
		if (!FontIsLocked) {
			FontPosition.x = x;
			FontPosition.y = y;
		}
	}

	void Font::update(int x, int y)
	{
		if (!FontIsLocked) {
			FontPosition.x = FontPosition.x + x;
			FontPosition.y = FontPosition.y + y;
		}
	}

	void Font::clean_up()
	{
		TTF_CloseFont(FontData);
	}

	FontManager::FontManager()
		: NumberOfFonts(-1), FontRenderer(NULL)
	{		
	}

	void FontManager::clean_up()
	{
		for (size_t i = 0; i <= NumberOfFonts; i++) {
			FontList[i]->clean_up();
			std::cout << "Cleaning Font: " << i << std::endl;
			delete FontList[i];
		}
	}

	unsigned int FontManager::create_font(SDL_Renderer* renderer, const std::string& filePath, const std::string& text, int size, const SDL_Color& c, int x, int y)
	{
		FontRenderer = renderer;
		NumberOfFonts++;

		FontList.push_back(new Font(filePath, text, size, c));

		unlock_font(NumberOfFonts);
		set_position(x, y, NumberOfFonts);
		if(FontList[NumberOfFonts]->Created)
			FontList[NumberOfFonts]->change_font(text, c, FontRenderer);
		lock_font(NumberOfFonts);

		return NumberOfFonts;
	}

	void FontManager::unlock_font(int ID)
	{
		FontList[ID]->FontIsLocked = false;
	}

	void FontManager::lock_font(int ID)
	{
		FontList[ID]->FontIsLocked = true;
	}

	void FontManager::change_font(const std::string& text, const SDL_Color& color, int ID)
	{
		FontList[ID]->change_font(text, color, FontRenderer);
	}

	void FontManager::render(int ID)
	{
		if (ID == -1) {
			for (size_t i = 0; i <= NumberOfFonts; i++) {
				FontList[i]->render(FontRenderer);
			}
		}
		else
			FontList[ID]->render(FontRenderer);
	}

	void FontManager::set_position(int x, int y, int ID)
	{
		FontList[ID]->set_position(x, y);
	}

	void FontManager::update_position(int dx, int dy, int ID)
	{
		FontList[ID]->update(dx, dy);
	}

	void FontManager::get_font_width_height(int* w, int* h, int ID)
	{
		TTF_SizeText(FontList[ID]->FontData, FontList[ID]->FontText.c_str(), w, h);
	}

	bool initialize_font()
	{
		if (TTF_Init() == -1)
			return false;

		std::cout << "Initialized SDL_ttf" << std::endl;
		return true;
	}

	bool initialize_mixer()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		std::cout << "Initialized SDL_Mixer" << std::endl;
		return false;
	}

	bool initialize_img()
	{
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;
		std::cout << "Initialized SDL_image" << std::endl;
		return true;
	}

	Image::Image(const std::string& filePath, int x, int y, int w, int h, SDL_Renderer* renderer)
		: Texture(load_texture(filePath, renderer)), Position({x, y, w, h}), TextureRenderer(renderer)
	{
		std::cout << "Created Image: " << filePath << std::endl;
	}

	Image::Image(const std::string& filePath, SDL_Rect Temp, SDL_Renderer* renderer)
		: Texture(load_texture(filePath, renderer)), Position(Temp), TextureRenderer(renderer)
	{
		std::cout << "Created Image: " << filePath << std::endl;
	}

	void Image::render(float angle, SDL_RendererFlip flip)
	{
		draw_no_source(Texture, TextureRenderer, Position, angle, flip);
	}

	void Image::set_image_alptha(int a)
	{
		set_alptha(Texture, a);
	}

	void Image::set_color(int r, int g, int b)
	{
		set_texture_color(Texture, r, g, b);
	}

	void Image::set_position(int x, int y)
	{
		Position.x = x;
		Position.y = y;
	}

	Image::~Image()
	{
		delete_texture(Texture);
		std::cout << "Deleted Image" << std::endl;
	}

	AudioChunk::AudioChunk(const std::string& path)
		: ChunkName(Mix_LoadWAV(path.c_str())), CurrentVolume(0)
	{
		if (ChunkName == NULL)
			std::cout << "Could Not Load Chunk: " << path << std::endl;
	}
	void AudioChunk::play_chunk()
	{
		Mix_PlayChannel(-1, ChunkName, 0);
	}

	void AudioChunk::set_volume(int volume)
	{
		CurrentVolume = (volume < 128) ? volume : 128;
		Mix_VolumeChunk(ChunkName, CurrentVolume);
	}

	AudioChunk::~AudioChunk()
	{
		Mix_FreeChunk(ChunkName);
	}

	AudioMusic::AudioMusic(const std::string& path)
		: Music(Mix_LoadMUS(path.c_str())), CurrentVolume(0)
	{
		if (Music == NULL)
			std::cout << "Could Not Load Music: " << path << std::endl;
	}

	void AudioMusic::play_music()
	{
		Mix_PlayMusic(Music, -1);
	}

	void AudioMusic::set_volume(int volume)
	{
		CurrentVolume = (volume < 128) ? volume : 128;
		Mix_VolumeMusic(CurrentVolume);
	}

	AudioMusic::~AudioMusic()
	{
		Mix_FreeMusic(Music);
	}

	SDL_Texture* load_texture(const std::string& filePath, SDL_Renderer* renderer)
	{
		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(filePath.c_str());
		if (Surface == NULL) {
			std::cout << "Could Not Load Texture: " << filePath << std::endl;
			SDL_Quit();
		}
		else
			std::cout << "Loaded Texture: " << filePath << std::endl;

		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(renderer, Surface);
		SDL_FreeSurface(Surface);

		return mainTexture;
	}

	SDL_Texture* create_texture_with_surface(SDL_Surface* surface, SDL_Renderer* renderer)
	{
		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		std::cout << "Created Texture From Surface" << std::endl;

		return mainTexture;
	}

	SDL_Surface* load_surface(const std::string& filePath)
	{
		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(filePath.c_str());
		if (Surface == NULL) {
			std::cout << "Could Not Load Surface: " << filePath << std::endl;
			SDL_Quit();
		}
		else
			std::cout << "Loaded Surface: " << filePath << std::endl;

		return Surface;
	}

	void draw_no_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_FRect& Destination, float angle, const SDL_RendererFlip& flip)
	{
		SDL_RenderCopyExF(renderer, Texture, NULL, &Destination, angle, NULL, flip);
	}

	void draw_with_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_FRect& Destination, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle)
	{
		SDL_RenderCopyExF(renderer, Texture, &src, &Destination, angle, NULL, flip);
	}

	void draw_no_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_Rect& Destination, float angle, const SDL_RendererFlip& flip)
	{
		SDL_RenderCopyEx(renderer, Texture, NULL, &Destination, angle, NULL, flip);
	}

	void draw_with_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_Rect& Destination, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle)
	{
		SDL_RenderCopyEx(renderer, Texture, &src, &Destination, angle, NULL, flip);
	}

	void set_window_icon(const std::string& filePath, SDL_Window* window)
	{
		SDL_Surface* icon = NULL;
		icon = IMG_Load(filePath.c_str());
		if (icon == NULL)
			std::cout << "Could Not Load Window Icon Surface:  " << filePath << std::endl;
		else
			std::cout << "Loaded Window Icon Surfcace: " << filePath << std::endl;
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	void delete_texture(SDL_Texture* tex)
	{
		SDL_DestroyTexture(tex);
	}

	void set_texture_color(SDL_Texture* tex, int r, int g, int b)
	{
		SDL_SetTextureColorMod(tex, r, g, b);
	}

	void set_alptha(SDL_Texture* tex, int a)
	{
		SDL_SetTextureAlphaMod(tex, a);
	}
}
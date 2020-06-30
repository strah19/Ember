#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>

namespace Ember
{

	class FontManager; //decleration of FontManager

	class Font //basic font class
	{
		Font(const std::string& filePath, const std::string& text, int size, const SDL_Color& c);
		void unlock_font();	//must unlock font to use properly
		void change_font(const std::string& Text, const SDL_Color& color, SDL_Renderer* renderer);
		void lock_font(); //de-activates fonts
		void render(SDL_Renderer* renderer); //does not need private render variable because class should not be used on client side
		void set_position(int x, int y);
		void update(int x, int y);
		void clean_up(); //must clean-up yourself to not have memory leak

		unsigned int FontID;
		int FontWidth;
		int FontHeight;
		std::string FontFilePath;
		TTF_Font* FontData;
		SDL_Rect FontPosition;
		SDL_Texture* FontTexture;
		SDL_Color FontColor;
		bool FontIsLocked;
		bool Created;
		std::string FontText;

		friend class FontManager; //friend class to allow manager to have access to all class
		FontManager* ParentFontManager = NULL; //pointer to manager
	};

	class FontManager
	{
	public:
		FontManager();
		unsigned int create_font(SDL_Renderer* renderer, const std::string& filePath, const std::string& text = "Hello World", int size = 32, const SDL_Color& c = { 0, 0, 0, 255 }, int x = 0, int y = 0);
		void unlock_font(int ID);
		void lock_font(int ID);
		void change_font(const std::string& text, const SDL_Color& color, int ID);
		void render(int ID = -1); //-1 means it will render all fonts in the manager
		void set_position(int x, int y, int ID);
		void update_position(int dx, int dy, int ID);
		void get_font_width_height(int* w, int* h, int ID);
		void clean_up(); //must clean-up yourself
	private:
		unsigned int NumberOfFonts;
		std::vector<Font*> FontList; //dynamic amount of fonts
		SDL_Renderer* FontRenderer;
	};

	//must call these if you want to use the code in this header file
	bool initialize_mixer();
	bool initialize_img();
	bool initialize_font();

	//basic class to make it easier to draw and use textures in SDL2
	class Image
	{
	public:
		Image(const std::string& filePath, int x, int y, int w, int h, SDL_Renderer* renderer);
		Image(const std::string& filePath, SDL_Rect Temp, SDL_Renderer* renderer);
		void render(float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void set_color(int r, int g, int b);
		void set_image_alptha(int a);
		void set_position(int x, int y);
		inline SDL_Texture* get_texture() { return Texture; } 
		~Image();
	private:
		SDL_Texture* Texture;
		SDL_Rect Position;
		SDL_Renderer* TextureRenderer;
	};

	//sound effects
	class AudioChunk
	{
	public:
		AudioChunk(const std::string& path);
		void play_chunk();
		void set_volume(int volume);
		~AudioChunk();
	private:
		Mix_Chunk* ChunkName;
		int CurrentVolume;
	};

	//background music
	class AudioMusic
	{
	public:
		AudioMusic(const std::string& path);
		void play_music(); //DO-NOT use this function in a loop
		void set_volume(int volume);
		~AudioMusic();
	private:
		Mix_Music* Music;
		int CurrentVolume;
	};

	SDL_Texture* load_texture(const std::string& filePath, SDL_Renderer* renderer); //if not using Image class can use this to return a texture
	SDL_Texture* create_texture_with_surface(SDL_Surface* surface, SDL_Renderer* renderer); //batch pixel maipulation to a texture
	SDL_Surface* load_surface(const std::string& filePath); //load surface for more pixel manipulation

	//can use a source or without and use floats or ints
	void draw_no_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_FRect& Destination, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE); 
	void draw_with_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_FRect& Destination, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);
	void draw_no_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_Rect& Destination, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	void draw_with_source(SDL_Texture* Texture, SDL_Renderer* renderer, const SDL_Rect& Destination, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);

	void set_window_icon(const std::string& filePath, SDL_Window* window);
	void delete_texture(SDL_Texture* tex); //wrapper if use load_texture
	void set_texture_color(SDL_Texture* tex, int r, int g, int b);
	void set_alptha(SDL_Texture* tex, int a);
}

#endif // !ASSET_LOADER_H

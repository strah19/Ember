#ifndef EMBER
#define EMBER

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
#include <unordered_map>

#define LeftMouseButton 1
#define MiddleMouseButton 2
#define RightMouseButton 3

#ifndef EMBER_ASSERT
#define EmberAssert(condition, message) \
        do { \
            if (! (condition)) { \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                          << " line " << __LINE__ << ": " << message << std::endl; \
                std::terminate(); \
            } \
        } while (false)
#endif 

namespace Ember
{
	void CheckVersion();
	SDL_DisplayMode GetDisplay();

	struct KeyBoardEvents
	{
		bool pressed;
		bool released;
		int repeat;
		std::string name;
		KeyBoardEvents()
			:pressed(false), released(false), repeat(0)
		{
		}
	};
	struct MouseEvents
	{
		int x, y;
		bool down;
		bool up;
		int id;
		int clicks;
		MouseEvents()
			:x(0), y(0), down(false), up(false), id(-1), clicks(0)
		{
		}
	};
	struct MouseMotionEvents
	{
		int xDirection;
		int yDirection;
		MouseMotionEvents()
			:xDirection(0), yDirection(0)
		{
		}
	};
	struct MouseWheelEvents
	{
		int xDirection;
		int yDirection;
		MouseWheelEvents()
			:xDirection(0), yDirection(0)
		{
		}
	};

	struct EventHandler
	{
		KeyBoardEvents keyBoard;
		MouseEvents mouse;
		MouseMotionEvents motion;
		MouseWheelEvents wheel;

		bool closeWithEscape;
		bool resize;

		EventHandler()
			:closeWithEscape(false), resize(false)
		{
		}
	};

	enum class EventFlags
	{
		escape, resize
	};

	class Cursor
	{
	public:
		Cursor();
		~Cursor();
		void set_cursor(SDL_SystemCursor type);

		void update();
		void show_cursor();
		void hide_cursor();
	private:
		SDL_Cursor* m_Cursor;
	};

	struct SDLProperties
	{
		std::string_view m_WindowTitle;
		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;
		SDL_Event m_Event;
		unsigned int m_Width;
		unsigned int m_Height;
		bool m_FullScreen;
		int m_WindowPositionX;
		int m_WindowPositionY;

		SDLProperties();
		SDLProperties(const char* title, unsigned int width, unsigned int height, bool full, int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED);
	};

	class EmberScreen
	{
	public:
		EmberScreen(SDLProperties* properties);
		~EmberScreen();
		bool IterateEvents();
		void AddEventFlags(EventFlags flag);
		void Close();
		inline SDLProperties* properties() { return m_Properties; }
		inline EventHandler* events() { return &m_HandleEvents; }
		inline void SetScreenSize(int w, int h) { SDL_SetWindowSize(m_Properties->m_Window, w, h); }
		bool m_IsRunning;
		Cursor cursor;
	private:
		SDLProperties* m_Properties;
		EventHandler m_HandleEvents;
	};

#ifndef ASSET_LOADER
	bool initialize_mixer();
	bool initialize_img();
	bool initialize_font();
	bool initialize_all_asset_loaders();

	class Font
	{
	public:
		Font(EmberScreen* screen, const char* filePath, const char* text, int size, const SDL_Color& color, int x, int y);
		void unlock_font();
		void change_font(const char* text, const SDL_Color& color);
		void lock_font();
		void render();
		void set_position(int x, int y);
		void update(int x, int y);
		void clean_up();
	private:
		int m_FontWidth;
		int m_FontHeight;
		TTF_Font* m_FontData;
		SDL_Rect m_FontPosition;
		SDL_Texture* m_FontTexture;
		SDL_Color m_FontColor;
		bool m_FontIsLocked;
		EmberScreen* m_CurrentScreen;
	};

	class Image
	{
	public:
		Image(EmberScreen* screen, const char* filePath, int x, int y, int w, int h);
		Image(EmberScreen* screen, const char* filePath, SDL_Rect position);
		void render(float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void set_color(int r, int g, int b);
		void set_image_alptha(int alpha);
		void set_position(int x, int y);
		inline SDL_Texture* get_texture() { return m_Texture; }
		~Image();
	private:
		SDL_Texture* m_Texture;
		SDL_Rect m_Position;
		EmberScreen* m_CurrentScreen;
	};

	class AudioChunk
	{
	public:
		AudioChunk(const char* filePath);
		void play_chunk();
		void set_volume(unsigned int volume);
		~AudioChunk();
	private:
		Mix_Chunk* m_ChunkName;
		unsigned int m_CurrentVolume;
	};

	class AudioMusic
	{
	public:
		AudioMusic(const char* filePath);
		void play_music(); 
		void set_volume(unsigned int volume);
		~AudioMusic();
	private:
		Mix_Music* m_Music;
		unsigned int m_CurrentVolume;
	};

	SDL_Texture* load_texture(EmberScreen* screen, const char* filePath);
	SDL_Texture* create_texture_with_surface(EmberScreen* screen, SDL_Surface* surface);
	SDL_Surface* load_surface(const char* filePath);

	void draw_no_source(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	void draw_with_source(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);
	void draw_no_source(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	void draw_with_source(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);

	void set_window_icon(EmberScreen* screen, const char* filePath);

	void delete_texture(SDL_Texture* texture);
	void set_texture_color(SDL_Texture* texture, int r, int g, int b);
	void set_alptha(SDL_Texture* texture, int a);
#endif

#ifndef SHAPES	
	void pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void horizontal_line(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void vertical_line(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void rectangle_border(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rectangle_fill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rectangle_borderF(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rectangle_fillF(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void draw_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void fill_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void arcs(SDL_Renderer* renderer, int x0, int y0, int radiusX, int radiusY, int quad1, int quad2, int quad3, int quad4, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	/*
	quad1 = TopRight
	quad2 = TopLeft
	quad3 = BottomLeft
	quad4 = BottomRight
	*/

	void rounded_rectangle_fill_gui(SDL_Renderer* renderer, int headerSize, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 rh, Uint8 gh, Uint8 bh);
	void rounded_rectangle_border_gui(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 rh, Uint8 gh, Uint8 bh);

	void rounded_rectangle_fill(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void rounded_rectangle_border(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
#endif 

#ifndef FUNCTION
	class Button
	{
	public:
		Button(EmberScreen* screen, SDL_Rect button);
		Button(EmberScreen* screen, int x, int y, int w, int h);
		bool hover();
		bool click(int ID);
		void update_position(int x, int y);
		void update_size(int w, int h);
		int in_out_click(int ID);
		bool hold(int ID);
		inline SDL_Rect retrieve_data() { return m_Position; }
	private:
		EmberScreen* m_CurrentScreen;
		SDL_Rect m_Position;
		int m_InOut;
		bool m_Clicked;
	};

	class EmberVec2
	{
	public:
		float x;
		float y;

		EmberVec2(const float x, const float y);
		EmberVec2() :x(0), y(0) {}

		friend std::ostream& operator<<(std::ostream& os, const EmberVec2& vec2);

		friend EmberVec2 operator+(const EmberVec2& v1, const EmberVec2& v2);
		friend EmberVec2 operator-(const EmberVec2& v1, const EmberVec2& v2);
		friend EmberVec2 operator*(const EmberVec2& v1, const EmberVec2& v2);
		friend EmberVec2 operator/(const EmberVec2& v1, const EmberVec2& v2);

		friend EmberVec2 operator+(const EmberVec2& v1, const float s);
		friend EmberVec2 operator-(const EmberVec2& v1, const float s);
		friend EmberVec2 operator*(const EmberVec2& v1, const float s);
		friend EmberVec2 operator/(const EmberVec2& v1, const float s);

		void operator+=(const float scalar);
		void operator-=(const float scalar);
		void operator*=(const float scalar);
		void operator/=(const float scalar);

		void operator+=(const EmberVec2& vec);
		void operator-=(const EmberVec2& vec);
		void operator*=(const EmberVec2& vec);
		void operator/=(const EmberVec2& vec);

		void negate();
		float magnitude();
		void normalize();
		float dot_product(EmberVec2& vec2);
	};

	EmberVec2 operator+(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator-(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator*(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator/(const EmberVec2& v1, const EmberVec2& v2);

	EmberVec2 operator+(const EmberVec2& v1, const float s);
	EmberVec2 operator-(const EmberVec2& v1, const float s);
	EmberVec2 operator*(const EmberVec2& v1, const float s);
	EmberVec2 operator/(const EmberVec2& v1, const float s);

	class Grid
	{
	public:
		Grid(EmberScreen* screen, int x, int y, unsigned int row, unsigned int col, int widthSize, int heightSize);
		void resize_define(int x, int y, int widthSize, int heightSize);
		void render_rects(Uint8 r, Uint8 g, Uint8 b);
		void render_border(Uint8 r, Uint8 g, Uint8 b);
		std::pair<int, int> hover();
		std::pair<int, int> click(int ID);

	protected:
		unsigned int m_Rows, m_Cols;
		int m_BlockWidth, m_BlockHeight;
		int m_X, m_Y;
		int m_StartX, m_StartY;

		EmberScreen* m_CurrentScreen;
		Button m_GridButtons;
	};

	class SpriteSheet
	{
	public:
		SpriteSheet(EmberScreen* screen, char const* filePath, int row, int column);
		SpriteSheet() = default;

		~SpriteSheet();

		void select_sprite(int x, int y);
		void draw_selected_sprite(SDL_Rect position);

		inline SDL_Texture* get_texture() const { return m_Sheet; }	
		inline int Row() { return m_Row; }
		inline int Col() { return m_Col; }
	private:
		SDL_Rect m_Clip;
		SDL_Texture* m_Sheet;

		int m_Row, m_Col;

		EmberScreen* m_CurrentScreen;
	};

	struct TileTexture
	{
		SDL_Texture* texture;
		uint32_t id;
	};

	class File
	{
	public:
		File(const char* filePath);
		virtual ~File();

		void empty_file();
		void close_file();
		void delete_file();

		inline const char* get_path() const { return m_FilePath; }
		inline std::fstream& get_file() { return m_FileData; }
		std::streampos get_current_location();
		bool check_if_file_empty();
		void change_file_location_pointer(int location);
		void reset_location();
		__int64 get_line_count();
		unsigned int get_word_count();
		unsigned int get_location_from_word(const char* word);
		inline unsigned long long get_size() { return m_CurrentSize; }
		std::string get_in_memory_data() const { return m_Data; }
		unsigned long get_character_count();
		bool is_empty() { return m_FileData.peek() == std::ifstream::traits_type::eof(); }

		//reading functions
		std::string read_all_file();
		std::string read_line(int lineNumber);
		std::string read_word(int location);
		std::string get_word_from_location(int location);

		//writing functions
		template <typename _Type>
		void write_at_end_of_file(const _Type& data, bool newLine);
		template <typename _Type>
		void write_at_end_of_word(const _Type& data, const char* finderWord);
		template <typename _Type>
		void write_at_end_of_line(const _Type& data, int lineNumber);
	protected:
		const char* m_FilePath;
		std::fstream m_FileData;
		unsigned long long m_CurrentSize;
		bool m_FillInMemoryData;
		std::string m_Data;

		std::streamoff get_size_in_bytes();
		void open();
		void before_reading();
		void before_writing();
	};

	bool BindFileToManager(File* file);
	void CheckForDoubles(const char* fileClosingFrom);
	void CloseAllFiles();

	class TileMap : public Grid
	{
	public:
		TileMap(EmberScreen* screen, int x, int y, unsigned int row, unsigned int col, int widthSize, int heightSize);
		~TileMap();

		void RenderSheet(SpriteSheet& spriteSheet, int x, int y);
		void UseSheet(SpriteSheet& spriteSheet);

		void RenderTexturePack();
		void AddTexture(const char* filePath);  
		void EditGrid();
	private:
		std::vector<TileTexture> m_Textures;
		uint32_t m_Counter;
		uint32_t m_CurrentTexture;

		SDL_Rect m_Pos;
		uint32_t** m_EachTileTexture;
		EmberVec2 m_LastPosition;

		//Sheet members
		uint32_t** m_EachTileSheet;
		EmberVec2 m_CurrentSheet;

		//Serialization
		File m_File;
	};

	class EmberVec4
	{
	public:
		EmberVec4(float x, float y, float w, float h)
			:a(x, y), b(w, h)
		{
		}
		EmberVec4()
			:a(0, 0), b(0, 0)
		{
		}
		union
		{
			struct
			{
				float x, y, w, h;
			};
			struct
			{
				EmberVec2 a, b;
			};
		};
	};

#endif
	
	struct EmberRect
	{
		EmberVec4 pos;
		EmberVec2 vel;
	};

	bool PointVsRect(const EmberVec2& point, const EmberRect& rect);
	bool RectVsRect(const EmberRect& rect1, const EmberRect& rect2);
	bool RayVsRect(const EmberVec2& ray, const EmberVec2& rayDir, const EmberRect& rect, EmberVec2&  contact, EmberVec2& contact_normal, float& hitNear);

	bool DynamicRectVsRect(const EmberRect& rect1, const EmberRect& rect2, EmberVec2& contact, EmberVec2& contact_normal, float& hitNear);
}

#endif
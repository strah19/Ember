#ifndef EMBER
#define EMBER

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#define LeftMouseButton 1
#define MiddleMouseButton 2
#define RightMouseButton 3

#define LeftMouseButtonX 4
#define RighttMouseButtonX 5

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
#ifdef EMBER_ASSERT
#define EmberAssert(condition, message)
#endif

namespace ember {
	extern void CheckVersion();
	extern SDL_DisplayMode GetDisplay();

	class EmberVec2 {
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

		friend bool operator!=(const EmberVec2& v1, const EmberVec2& v2);

		void Negate();
		float Magnitude();
		EmberVec2 Normalize();
		float DotProduct(EmberVec2& vec2);
	};

	EmberVec2 operator+(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator-(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator*(const EmberVec2& v1, const EmberVec2& v2);
	EmberVec2 operator/(const EmberVec2& v1, const EmberVec2& v2);

	EmberVec2 operator+(const EmberVec2& v1, const float s);
	EmberVec2 operator-(const EmberVec2& v1, const float s);
	EmberVec2 operator*(const EmberVec2& v1, const float s);
	EmberVec2 operator/(const EmberVec2& v1, const float s);

	bool operator!=(const EmberVec2& v1, const EmberVec2& v2);

	class EmberVec4 {
	public:
		EmberVec4(float x, float y, float w, float h);
		EmberVec4();

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

	struct Circle {
		int radius;
		int x, y;
	};

	struct CircleF {
		float radius;
		float x, y;
	};

	struct EmberRect {
		EmberVec4 pos;
		EmberVec2 vel;
	};

	struct KeyBoardEvents {
		bool pressed;
		bool released;
		int repeat;
		std::string name;
		KeyBoardEvents();
	};

	struct MouseEvents {
		int x, y;
		bool down;
		bool up;
		int id;
		int clicks;
		int state;
		
		MouseEvents();
	};

	struct MouseMotionEvents {
		int x_direction;
		int y_direction;
		MouseMotionEvents();
	};

	struct MouseWheelEvents {
		int x_direction;
		int y_direction;
		MouseWheelEvents();
	};

	struct JoyStickEvents {
		bool down;
		bool up;
		int button_id;
		long int value;
		int axis_id;

		JoyStickEvents();
	};

	struct EventHandler {
    	KeyBoardEvents keyBoard;
		MouseEvents mouse;
		MouseMotionEvents motion;
		MouseWheelEvents wheel;
		JoyStickEvents joystick;

		bool closeWithEscape;
		bool resize;
		bool joystick_active;

		EventHandler();
	};

	enum class EventFlags {
		kEscape, kResize, kJoystick
	};

	class Cursor {
	public:
		Cursor();
		~Cursor();
		void SetCursor(SDL_SystemCursor cursor_type);

		void Update();
		void ShowCursor();
		void HideCursor();
	private:
		SDL_Cursor* m_cursor;
	};

	struct SDLProperties {
		std::string_view window_title;
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event event;
		int width;
		int height;
		bool full_screen;
		int window_x_position;
		int window_y_position;

		SDLProperties();
		SDLProperties(const char* window_title, int width, int height, bool full_screen, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED);
	};

	class EmberScreen
	{
	public:
		EmberScreen(SDLProperties* properties);
		~EmberScreen();
		bool IterateEvents();
		void AddEventFlags(EventFlags event_flag);
		void Close();

		inline SDLProperties* Properties() { return m_properties; }
		inline EventHandler* Events() { return &m_handle_events; }
		inline void SetScreenSize(int screen_w, int screen_h) { SDL_SetWindowSize(m_properties->window, screen_w, screen_h); }

		bool Running;
		Cursor Cursor;

		void StartTime();
		float DeltaTime() { return m_delta_time; }

		bool KeyboardState(SDL_Scancode scancode);

		void Brightness(float brightness) { SDL_SetWindowBrightness(Properties()->window, brightness); }
		void Title(const char* title) { SDL_SetWindowTitle(Properties()->window, title); Properties()->window_title = title; }
		void Border(SDL_bool bordered) { SDL_SetWindowBordered(Properties()->window, bordered); }
	private:
		SDLProperties* m_properties;
		EventHandler m_handle_events;
		Uint32 m_latest;
		float m_delta_time;
		SDL_Joystick* m_joy;
	};

	static bool InitializeMixer();
	static bool InitializeImg();
	static bool InitializeFonts();
	static bool InitializeAllAssets();

	class Font {
	public:
		Font(EmberScreen* screen, const char* file_path, const char* text, int size, const SDL_Color& color, int x, int y);
		void UnlockFont();

		template <typename T>
		void ChangeFont(const T& text, const SDL_Color& color) {
			if (!m_font_is_locked) {
				std::stringstream ss;
				ss << text;
				m_font_color = color;
				SDL_DestroyTexture(m_font_texture);
				SDL_Surface* fontSurface = TTF_RenderText_Blended(m_font, ss.str().c_str(), m_font_color);
				m_font_width = fontSurface->w;
				m_font_height = fontSurface->h;

				m_font_position.w = m_font_width;
				m_font_position.h = m_font_height;

				m_font_texture = SDL_CreateTextureFromSurface(m_screen->Properties()->renderer, fontSurface);

				SDL_FreeSurface(fontSurface);
			}
		}

		void LockFont();
		void Render();
		void SetPosition(int x, int y);
		inline void SetSize(int w, int h) { m_font_width = w; m_font_height = h; }
		void Update(int vel_x, int vel_y);
		void CleanUp();
		inline void GetSize(int* w, int* h, const char* text) { TTF_SizeText(m_font, text, w, h); }
	private:
		int m_font_width;
		int m_font_height;
		TTF_Font* m_font;
		SDL_Rect m_font_position;
		SDL_Texture* m_font_texture;
		SDL_Color m_font_color;
		bool m_font_is_locked;
		EmberScreen* m_screen;
	};

	class Image {
	public:
		Image(EmberScreen* screen, const char* file_path, int x, int y, int w, int h);
		Image(EmberScreen* screen, const char* file_path, SDL_Rect& position);
		void Render(float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void SetColor(int r, int g, int b);
		void SetImageAlptha(int alptha);
		void SetPosition(int x, int y);
		inline SDL_Texture* get_texture() { return m_texture; }
		~Image();
	private:
		SDL_Texture* m_texture;
		SDL_Rect m_position;
		EmberScreen* m_screen;
	};

	class AudioChunk {
	public:
		AudioChunk(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		~AudioChunk();
	private:
		Mix_Chunk* m_chunk;
		unsigned int m_volume;
	};

	class AudioMusic {
	public:
		AudioMusic(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		~AudioMusic();
	private:
		Mix_Music* m_music;
		unsigned int m_volume;
	};

	SDL_Texture* LoadTexture(EmberScreen* screen, const char* file_path);
	SDL_Texture* CreateTextureFromSurface(EmberScreen* screen, SDL_Surface* surface);
	SDL_Surface* LoadSurface(const char* file_path);

	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);
	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, float angle = 0, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0);

	void SetWindowIcon(EmberScreen* screen, const char* file_path);

	void DeleteTexture(SDL_Texture* texture);
	void TextureColor(SDL_Texture* texture, int r, int g, int b);
	void TextureAlptha(SDL_Texture* texture, int a);
	void GetTextureInfo(SDL_Texture* texture, int& w, int& h);

	void Pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void HorizontalLine(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void Line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void VerticalLine(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void Pixel(SDL_Renderer* renderer, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void HorizontalLine(SDL_Renderer* renderer, float x1, float x2, float y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void Line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void VerticalLine(SDL_Renderer* renderer, float y1, float y2, float x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void RectangleBorder(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void RectangleFill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void RectangleBorder(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void RectangleFill(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void FillCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

	bool PointVsRect(const EmberVec2& point, const EmberRect& rect);
	bool RectVsRect(const EmberRect& rect1, const EmberRect& rect2);
	bool RayVsRect(const EmberVec2& ray, const EmberVec2& ray_dir, const EmberRect& rect, EmberVec2& contact, EmberVec2& contact_normal, float& hit_near);
	bool DynamicRectVsRect(const EmberRect& rect1, const EmberRect& rect2, EmberVec2& contact, EmberVec2& contact_normal, float& hit_near);
	bool PointToCircle(EmberVec2& point, Circle& circle);
	bool PointToCircle(EmberVec2& point, CircleF& circle);

	void BeganRender(EmberScreen* screen, Uint8 r, Uint8 g, Uint8 b);
	void CloseRender(EmberScreen* screen);

	void Shadow(SDL_Renderer* renderer, SDL_Rect& position, int depth, SDL_Color start_color, int color_change);
	void Curve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color);
	void AdvCurve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color);

	class Button {
	public:
		Button(EmberScreen* screen, SDL_Rect& button);
		Button(EmberScreen* screen, int x, int y, int w, int h);
		bool Hover();
		bool Click(int id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);
		bool Hold(int id);
		bool InOut(int id);
		inline SDL_Rect Rect() { return m_position; }
	protected:
		SDL_Rect m_position;
		EmberScreen* m_screen;
		bool m_in;
		bool m_clicked;
	};

	class Grid {
	public:
		Grid(EmberScreen* screen, int x, int y, unsigned int col, unsigned int row, int width_size, int height_size);
		virtual void ResizeGrid(int x, int y, unsigned int col, unsigned int row, int width_size, int height_size);
		void RenderRect(unsigned int col, unsigned int row, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
		void RenderBorder(Uint8 r, Uint8 g, Uint8 b);
		EmberVec2 Hover();
		EmberVec2 Click(int id);
		EmberVec2 Hold(int id);

		template <typename T>
		void Personalize(T& function) {
			m_x = m_start_x;
			m_y = m_start_y;

			for (unsigned int i = 0; i < m_cols; i++) {
				for (unsigned int j = 0; j < m_rows; j++) {
					function(m_x, m_y, m_block_width, m_block_height);

					m_y += m_block_height;
				}
				m_x += m_block_width;
				m_y = m_start_y;
			}
		}

		EmberVec2 ConvertPoints(unsigned int col, unsigned int row);
	protected:
		unsigned int m_rows, m_cols;
		int m_block_width, m_block_height;
		int m_x, m_y;
		int m_start_x, m_start_y;

		EmberScreen* m_screen;
		Button m_button;
	};

	class SpriteSheet {
	public:
		SpriteSheet(EmberScreen* screen, char const* file_path, int row, int column);
		SpriteSheet() = default;

		~SpriteSheet();

		void SelectSprite(int x, int y);
		void DrawSelectedSprite(SDL_Rect& position);

		inline SDL_Texture* Texture() const { return m_sheet; }
		inline int Row() { return m_row; }
		inline int Col() { return m_col; }
	private:
		SDL_Rect m_clip;
		SDL_Texture* m_sheet;

		int m_row, m_col;

		EmberScreen* m_screen;
	};

	class Camera
	{
	public:
		Camera(EmberScreen* screen);
		Camera(EmberScreen* screen, float offset_x, float offset_y);
		void Pan(int button_click);
		void Scale(float scale_x, float scale_y);
		void MoveCameraOffset(float offset_x, float offset_y);

		void WorldToScreen(float f_world_x, float f_world_y, int& screen_x, int& screen_y);
		void ScreenToWorld(int screen_x, int screen_y, float& f_world_x, float& f_world_y);

		void RenderBoundry();
		EmberVec2 Offset() { return { m_offset_x, m_offset_y }; }
		EmberVec2 ScaleValue() { return { m_scale_x, m_scale_y }; }

	private:
		float m_offset_x, m_offset_y;
		float m_scale_x, m_scale_y;
		EmberScreen* m_screen;
		bool m_down_flag;
		float m_pan_x, m_pan_y;
	};

	class Timer {
	public:
		Timer();
		int StartStopWatch();
		bool CreateTimer(unsigned int time);
	private:
		unsigned int m_current_time, m_last_time;
		int m_second_pass;
	};
}

#endif
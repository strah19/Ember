#ifndef EMBER
#define EMBER

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <memory>
#include <array>
#include <bitset>

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

namespace ember {
	extern void CheckVersion();
	extern SDL_DisplayMode GetDisplay();

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

	struct EventHandler {
    	KeyBoardEvents keyBoard;
		MouseEvents mouse;
		MouseMotionEvents motion;
		MouseWheelEvents wheel;

		bool closeWithEscape;
		bool resize;

		EventHandler();
	};

	enum class EventFlags {
		kEscape, kResize
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

		bool m_is_running;
		Cursor m_cursor;

		void StartTime();
		float DeltaTime() { return m_delta_time; }
	private:
		SDLProperties* m_properties;
		EventHandler m_handle_events;
		Uint32 m_latest;
		float m_delta_time;
	};

	bool InitializeMixer();
	bool InitializeImg();
	bool InitializeFonts();
	bool InitializeAllAssets();

	class Font {
	public:
		Font(EmberScreen* screen, const char* file_path, const char* text, int size, const SDL_Color& color, int x, int y);
		void UnlockFont();
		void ChangeFont(const char* text, const SDL_Color& color);
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

	class Button {
	public:
		Button(EmberScreen* screen, SDL_Rect& button);
		Button(EmberScreen* screen, int x, int y, int w, int h);
		bool Hover();
		bool Click(int id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);
		bool Hold(int id);
		inline SDL_Rect Rect() { return m_position; }
	protected:
		SDL_Rect m_position;
		EmberScreen* m_screen;

	};

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

	class EmberIVec2 {
	public:
		int x;
		int y;

		EmberIVec2(int x, int y);
		EmberIVec2();

		friend std::ostream& operator<<(std::ostream& os, const EmberIVec2& vec2);

		friend EmberIVec2 operator+(const EmberIVec2& v1, const EmberIVec2& v2);
		friend EmberIVec2 operator-(const EmberIVec2& v1, const EmberIVec2& v2);
		friend EmberIVec2 operator*(const EmberIVec2& v1, const EmberIVec2& v2);
		friend EmberIVec2 operator/(const EmberIVec2& v1, const EmberIVec2& v2);

		friend EmberIVec2 operator+(const EmberIVec2& v1, const int s);
		friend EmberIVec2 operator-(const EmberIVec2& v1, const int s);
		friend EmberIVec2 operator*(const EmberIVec2& v1, const int s);
		friend EmberIVec2 operator/(const EmberIVec2& v1, const int s);

		void operator+=(const int scalar);
		void operator-=(const int scalar);
		void operator*=(const int scalar);
		void operator/=(const int scalar);

		void operator+=(const EmberIVec2& vec);
		void operator-=(const EmberIVec2& vec);
		void operator*=(const EmberIVec2& vec);
		void operator/=(const EmberIVec2& vec);
	};

	EmberIVec2 operator+(const EmberIVec2& v1, const EmberIVec2& v2);
	EmberIVec2 operator-(const EmberIVec2& v1, const EmberIVec2& v2);
	EmberIVec2 operator*(const EmberIVec2& v1, const EmberIVec2& v2);
	EmberIVec2 operator/(const EmberIVec2& v1, const EmberIVec2& v2);

	EmberIVec2 operator+(const EmberIVec2& v1, const int s);
	EmberIVec2 operator-(const EmberIVec2& v1, const int s);
	EmberIVec2 operator*(const EmberIVec2& v1, const int s);
	EmberIVec2 operator/(const EmberIVec2& v1, const int s);

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

	class Grid {
	public:
		Grid(EmberScreen* screen, int x, int y, unsigned int col, unsigned int row, int width_size, int height_size);
		void ReSizeGrid(int x, int y, int width_size, int height_size);
		void RenderRect(unsigned int col, unsigned int row, Uint8 r, Uint8 g, Uint8 b);
		void RenderBorder(Uint8 r, Uint8 g, Uint8 b);
		EmberIVec2 Hover();
		EmberIVec2 Click(int id);
	protected:
		unsigned int m_rows, m_cols;
		int m_block_width, m_block_height;
		int m_x, m_y;
		int m_start_x, m_start_y;

		EmberScreen* m_screen;
		Button m_button;
	};

	class TileMap : public Grid {
	public:
		TileMap(EmberScreen* screen, int x, int y, unsigned int col, unsigned int row, int width_size, int height_size);
	private:

	};
	
	struct EmberRect {
		EmberVec4 pos;
		EmberVec2 vel;
	};

	bool PointVsRect(const EmberVec2& point, const EmberRect& rect);
	bool RectVsRect(const EmberRect& rect1, const EmberRect& rect2);
	bool RayVsRect(const EmberVec2& ray, const EmberVec2& ray_dir, const EmberRect& rect, EmberVec2&  contact, EmberVec2& contact_normal, float& hit_near);

	bool DynamicRectVsRect(const EmberRect& rect1, const EmberRect& rect2, EmberVec2& contact, EmberVec2& contact_normal, float& hit_near);

	void BeganRender(EmberScreen* screen, Uint8 r, Uint8 g, Uint8 b);
	void CloseRender(EmberScreen* screen);

	class Component;
	class Entity;

	using ComponentID = std::size_t;

	inline ComponentID GetComponentTypeID() {
		static ComponentID current_component_id = 0;
		return current_component_id++;
	}

	template <typename T> inline ComponentID GetComponentTypeID() {
		static ComponentID typeID = GetComponentTypeID();
		return typeID;
	}

	constexpr std::size_t kMaxComponents = 32;

	class Component {
	public:
		Component();
		virtual ~Component();

		virtual void Init();
		virtual void Update();
		virtual void Render();

		Entity* m_parent_entity;
	};

	class Entity {
	public:
		Entity(EmberScreen* screen);

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs) {
			T* component(new T(std::forward<TArgs>(mArgs)...));
			component->m_parent_entity = this;

			std::unique_ptr<Component> component_pointer{ component };
			m_components.emplace_back(std::move(component_pointer));

			m_component_array[GetComponentTypeID<T>()] = component;
			m_component_set[GetComponentTypeID<T>()] = true;
			
			component->Init();
			return *component;
		}

		template <typename T>
		T& GetComponent() {
			auto component_pointer(m_component_array[GetComponentTypeID<T>()]);
			return *static_cast<T*>(component_pointer);
		}

		template<typename T> bool HasComponent() {
			return m_component_set[GetComponentTypeID<T>];
		}

		void Update();
		void Render();

		inline EmberScreen* GetScreen() { return m_screen; }
		inline void Debug(bool is_active) { m_debug_is_active = is_active; }
		inline bool Debug() { return m_debug_is_active; }
	private:
		std::vector<std::unique_ptr<Component>> m_components;
		std::bitset<kMaxComponents> m_component_set;
		std::array<Component*, kMaxComponents> m_component_array;
		EmberScreen* m_screen;
		bool m_debug_is_active;
	};

	class PositionComponent : public Component {
	public:
		PositionComponent(float x, float y, float w, float h);
		PositionComponent(SDL_FRect position);
		virtual ~PositionComponent();

		void Init();
		void Update();
		void Render();

		void ChangePosition(float x, float y);
		void ChangeSize(float w, float h);
		void UpdatePosition(float dx, float dy);
		void UpdateSize(float dw, float dh);
		void Angle(float angle);

		inline SDL_FRect Position() { return m_position; }
		inline float GetAngle() { return m_angle; }

		void Scale(int on_x, int on_y);
	private:
		SDL_FRect m_position;
		float m_angle;
		EmberVec2 m_velocity;
	};

	class SpriteComponent : public Component {
	public:
		SpriteComponent(const char* file_path);
		virtual ~SpriteComponent(); 

		void Init();
		void Update();
		void Render();

		void Flip(SDL_RendererFlip flip);
		void SetColor(Uint8 r, Uint8 g, Uint8 b);
		void SetAlptha(Uint8 alptha);

		void ChangeSpriteClipping(int x, int y, int w, int h);
	private:
		PositionComponent* m_position;
		EmberScreen* m_screen;
		SDL_Texture* m_texture;
		SDL_RendererFlip m_flip;
		SDL_Rect m_clipping;

		const char* m_file_path;
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


	class CameraComponent : public Component {
	public:
		CameraComponent(Camera* camera);
		virtual ~CameraComponent();

		void Init();
		void Update();
		void Render();
	private:
		PositionComponent* m_position;
		Camera* m_camera;
	};

	struct Circle {
		int radius;
		int x, y;
	};

	struct CircleF {
		float radius;
		float x, y;
	};

	bool PointToCircle(EmberIVec2& point, Circle& circle);
	bool PointToCircle(EmberVec2& point, CircleF& circle);

	void Shadow(SDL_Renderer* renderer, SDL_Rect& position, int depth, SDL_Color start_color, int color_change);
	void Curve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color);
	void AdvCurve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color);

	struct EmberTheme {
		SDL_Color border_color;
		SDL_Color main_color;
		SDL_Color font_color;

		SDL_Color border_hover_color;
		SDL_Color main_hover_color;
		SDL_Color font_hover_color;
	
		EmberTheme() = default;
	};

	struct EmberButtonTheme : public EmberTheme{
		SDL_Color border_click_color;
		SDL_Color main_click_color;
		SDL_Color font_click_color;

		EmberButtonTheme() = default;
	};

	class EmberButton : public Button {
	public:
		EmberButton(EmberScreen* screen, EmberIVec2& position, const char* file_path, int size);
		EmberButton(EmberScreen* screen, EmberIVec2& position, const char* file_path, EmberButtonTheme& theme, int size);

		virtual ~EmberButton();

		void Render(const char* text, int id, int w_gap, int h_gap, int w_gap_s = -1, int h_gap_s = -1);

		void HoverColor(SDL_Color border_c, SDL_Color main_c, SDL_Color font_c);
		void MainColor(SDL_Color border_c, SDL_Color main_c, SDL_Color font_c);
		void ClickColor(SDL_Color border_c, SDL_Color main_c, SDL_Color font_c);

		void LockText(bool lock) { (lock) ? m_font.LockFont() : m_font.UnlockFont(); }
		void Border(bool border) { m_border = border; }
		void InOut(bool in) { m_is_using_in = in; }
		bool InOut() { return m_in; }
		void ActivateClick(bool c) { m_click = c; }
		void Main(bool m) { m_main = m; }

		void OverRideWidth(int w) { m_max_sizes.x = w; }
		void OverRideHeight(int h) { m_max_sizes.y = h; }
	private:
		Font m_font;
		EmberButtonTheme m_theme;

		void ThemeInit();
		bool m_in;
		bool m_border;
		bool m_main;
		bool m_is_using_in;
		bool m_click;

		EmberIVec2 m_max_sizes;
	};
}

#endif
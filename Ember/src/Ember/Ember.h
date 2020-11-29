#ifndef EMBER_H
#define EMBER_H

#include <functional>
#include <fstream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "Vector.h"
#include "File.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define EMBER_BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef _WIN32
#ifdef _WIN64
#error "Ember does not support x64."
#else
#define EMBER_WINDOW_PLATFORM
#endif

#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define HZ_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define HZ_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif

#elif defined(__ANDROID__)
#define HZ_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define HZ_PLATFORM_LINUX
#error "Linux is not supported!"
#else
#error "Unknown platform!"
#endif

namespace Ember {
	struct Rect {
		union {
			struct {
				int x, y, w, h;
			};
			struct {
				IVec2 pos, size;
			};
			struct {
				SDL_Rect rect;
			};
		};

		Rect() { rect = { 0, 0, 0, 0 }; }
		Rect(int x, int y, int w, int h) { rect = { x, y, w, h }; }
		Rect(const Rect& copy_rect) { rect = copy_rect.rect; }
		Rect(SDL_Rect sdl_rect) { rect = sdl_rect; }
	};

	struct FRect {
		union {
			struct {
				float x, y, w, h;
			};
			struct {
				Vec2 pos, size;
			};
			struct {
				SDL_FRect rect;
			};
		};

		FRect() { rect = { 0, 0, 0, 0 }; }
		FRect(float x, float y, float w, float h) { rect = { x, y, w, h }; }
		FRect(FRect& copy_rect) { rect = copy_rect.rect; }
		FRect(SDL_FRect sdl_rect) { rect = sdl_rect; }
	};

	struct Color {
		union {
			struct {
				Uint8 r, g, b, a;
			};
			struct {
				SDL_Color color;
			};
		};
	};

	class Layer {
	public:
		Layer(const std::string& name) : layer_name(name) { }
		std::string Name() { return layer_name; }
	private:
		std::string layer_name;
	};

	class EventStack {
	public:
		enum class EventStackError {
			LayerNameCouldNotBeFound, LayerDeletionFailed, DuplicitLayerFound, Null
		};

		EventStack(const std::string& first_name);
		EventStackError SetCurrentLayer(const std::string& name);
		EventStackError AddLayer(const std::string& name);
		EventStackError DeleteLayer(const std::string& name);
		EventStackError DrawEventToLayer(const std::function<void()>& event_func, const std::string& name);
	private:
		size_t IndexLayers(const std::string& name);

		size_t current_layer;
		std::vector<Layer> event_layers;
	};

	bool InitializeImageLoader();
	bool InitializeFontLoader();
	bool InitializeSoundLoader();
	bool InitializeAssets();
	void AssetCleanUp();

	SDL_DisplayMode GetDisplay();
	SDL_bool ConvertToSDLBool(bool value);
	void CheckVersion();

	struct WindowProperties {
		std::string name;
		int width;
		int height;
		bool full_screen;
		IVec2 position;
		WindowProperties()
			: name(), width(0), height(0), position(-1, -1), full_screen(false) { }
		WindowProperties(std::string name, int width, int height)
			: name(name), width(width), height(height), position(-1, -1), full_screen(false) { }
	};

	class Window {
	public:
		virtual ~Window() = default;

		virtual SDL_Window* GetNativeWindow() const = 0;
		virtual bool IsRunning() const = 0;
		virtual void Quit() = 0;
		virtual void Update() = 0;

		virtual WindowProperties* Properties() = 0;
	};

#ifdef EMBER_WINDOW_PLATFORM
	class wWindow : public Window {
	public:
		wWindow(WindowProperties* properties);
		virtual ~wWindow();

		virtual SDL_Window* GetNativeWindow() const override { return native_window; }
		virtual void Update();

		void SetBorder(bool border) { SDL_SetWindowBordered(native_window, ConvertToSDLBool(border)); }
		void SetBrightness(float brightness) { SDL_SetWindowBrightness(native_window, brightness); }
		void SetWindowIcon(const char* file_path);

		virtual bool IsRunning() const override { return is_running; }
		void Quit() { is_running = false; }

		virtual WindowProperties* Properties() { return properties; }

	private:
		WindowProperties* properties;
		SDL_Window* native_window;

		bool is_running;

		bool Initializer(WindowProperties* properties);
		void Destroy();
		bool AssertProperties();

		bool CreateWindow(WindowProperties* properties);
		Uint32 IsFullScreen(WindowProperties* properties);
		bool AssertSize(uint32_t width, uint32_t height);
		void CalculatePosition(IVec2* position);
	};
#endif // EMBER_WINDOW_PLATFORM

	class rRenderer {
	public:
		rRenderer(Window* window);
		virtual ~rRenderer();

		SDL_Renderer* Renderer();

		void SetViewport(int x, int y, int w, int h);

		void Clear(const Color& color);
		void SetColor(const Color& color);
		void Show();

		void Rectangle(const Rect& rect, const Color& color);
		void Border(const Rect& rect, const Color& color);
		void Line(const IVec2& point1, const IVec2& point2, const Color& color);
		void Point(const IVec2& point, const Color& color);

		void RectangleF(const FRect& rect, const Color& color);
		void BorderF(const FRect& rect, const Color& color);
		void LineF(const Vec2& point1, const Vec2& point2, const Color& color);
		void PointF(const Vec2& point, const Color& color);
	private:
		SDL_Renderer* renderer;
		Window* window;

		bool Initializer();
		void Destroy();
	};

	class Texture {
	public:
		Texture(const char* file_path, rRenderer* renderer);
		~Texture();

		void Draw(const FRect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);
		void Draw(const Rect& dest, float angle = 0.0f, const SDL_RendererFlip& flip = SDL_FLIP_NONE);
		void Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip = SDL_FLIP_NONE, float angle = 0.0f);

		static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* file_path);
		static SDL_Surface* LoadSurface(const char* file_path);
		static SDL_Texture* CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);

		void DeleteTexture();

		void TextureColor(const Color& color);
		void TextureAlptha(Uint8 a);
		IVec2 GetTextureInfo();
	private:
		SDL_Texture* texture;
		rRenderer* renderer;
	};

	class Font {
	public:
		Font(rRenderer* renderer, const char* file_path, const char* text, int size, const Color& color, int x, int y);
		void UnlockFont();

		template <typename T>
		void ChangeFont(const T& text, const Color& color) {
			if (!font_is_locked) {
				std::stringstream ss;
				ss << text;
				font_color = color;
				SDL_DestroyTexture(font_texture);
				SDL_Surface* fontSurface = TTF_RenderText_Blended(font, ss.str().c_str(), font_color.color);
				font_width = fontSurface->w;
				font_height = fontSurface->h;

				font_position.w = font_width;
				font_position.h = font_height;

				font_texture = SDL_CreateTextureFromSurface(renderer->Renderer(), fontSurface);

				SDL_FreeSurface(fontSurface);
			}
		}

		void LockFont();
		void Render();
		void SetPosition(int x, int y);
		inline void SetSize(int w, int h) { font_width = w; font_height = h; }
		void Update(int vel_x, int vel_y);
		inline void GetSize(int* w, int* h, const char* text) { TTF_SizeText(font, text, w, h); }
		void Destroy() { TTF_CloseFont(font); }
		rRenderer* Renderer() { if (renderer != nullptr) return renderer; }
	private:
		int font_width;
		int font_height;
		TTF_Font* font;
		SDL_Rect font_position;
		SDL_Texture* font_texture;
		Color font_color;
		bool font_is_locked;
		rRenderer* renderer;
	};

	class AudioChunk {
	public:
		AudioChunk(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		virtual ~AudioChunk();
	private:
		Mix_Chunk* m_chunk;
		unsigned int m_volume;
	};

	class AudioMusic {
	public:
		AudioMusic(const char* file_path);
		void Play();
		void Volume(unsigned int volume);
		virtual ~AudioMusic();
	private:
		Mix_Music* m_music;
		unsigned int m_volume;
	};

	struct Event {
	public:
		Event(const std::string& name)
			: active(true), Handled(false), name(name) { }
		virtual ~Event() = default;

		bool ActivityCheck() const { return active; }
		void Active(bool active) { this->active = active; }

		bool Handled;

		virtual std::string GetName() const { return ""; }
	protected:
		bool active;
		std::string name;
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event* event)
			: event(event) {
		}

		virtual ~EventDispatcher() {
			event = nullptr;
		}

		template<typename T>
		bool Dispatch(const std::function<bool(T&)> func) {
			if (event->ActivityCheck() && dynamic_cast<T*>(event)) {
				event->Handled = func(static_cast<T&>(*event));
				return true;
			}
			return false;
		}

		inline Event* GetEvent() { return event; }
	private:
		Event* event;
	};

	struct KeyboardEvents : public Event {
		KeyboardEvents(bool pressed, int repeat, int scancode)
			: Event("Keyboard"), pressed(pressed), repeat(repeat), scancode(scancode) { }
		virtual ~KeyboardEvents() = default;

		std::string GetName() const { return name; }

		bool pressed;
		int repeat;
		int scancode;
	};

	enum class ButtonIds {
		LeftMouseButton = 1, MiddleMouseButton = 2, RightMouseButton = 3
	};
	struct MouseButtonEvents : public Event {
		MouseButtonEvents(bool down, int id, int clicks)
			: Event("Mouse"), down(down), button_id(static_cast<ButtonIds>(id)), clicks(clicks) { }
		virtual ~MouseButtonEvents() = default;

		std::string GetName() const { return name; }

		bool down;
		ButtonIds button_id;
		int clicks;
	};

	struct MouseButtonPositionEvents : public Event {
		MouseButtonPositionEvents(int x, int y)
			: Event("Position"), position(x, y) { }
		virtual ~MouseButtonPositionEvents() = default;

		std::string GetName() const { return name; }

		IVec2 position;
	};

	struct QuitEvent : public Event {
		QuitEvent(bool closed)
			: Event("Quit"), closed(closed) { }
		virtual ~QuitEvent() = default;

		std::string GetName() const { return name; }

		bool closed;
	};

	struct ResizeEvent : public Event {
		ResizeEvent(int w, int h)
			: Event("Resize"), w(w), h(h) { }
		virtual ~ResizeEvent() = default;

		std::string GetName() const { return name; }

		int w, h;
	};

	using Callback = std::function<void(Event&)>;

	class Events {
	public:
		Events()
			: mouse_event(false, 0, 0), keyboard_event(false, 0, 0), mouse_pos(0, 0) { }

		bool Down() const { return mouse_event.down; }
		IVec2 MousePosition() const { return mouse_pos.position; }
		ButtonIds ButtonId() const { return mouse_event.button_id; }
		int KeyCode() { return keyboard_event.scancode; }
		bool KeyDown() { return keyboard_event.pressed; }
	private:
		MouseButtonEvents mouse_event;
		KeyboardEvents keyboard_event;
		MouseButtonPositionEvents mouse_pos;
		friend class EventHandler;
	};

	class EventHandler {
	public:
		EventHandler(Window* window, Events* events);

		void Update();
		void SetEventCallback(const Callback& callback) { this->callback = callback; }

		SDL_Event NativeEvent() const { return native_event_handler; }
	private:
		Window* window;
		SDL_Event native_event_handler;
		Callback callback;
		Events* events;

		void Quit();
		void KeyEvent();
		void Resize();
		void MouseEvent();
		void MousePositonEvent();
	};

	class Application {
	public:
		Application(const std::string& name = "Game Engine", bool full_screen = false, uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application();

		void Run();

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUserUpdate() { }
		virtual void OnCreate() { }

		wWindow* GetWindow() { return window; }
		Events* GetEvents() { return events; }
		rRenderer* GetRenderer() { return renderer; }
	protected:
#ifdef EMBER_WINDOW_PLATFORM
		wWindow* window;
#endif // EMBER_WINDOW_PLATFORM
		EventHandler* event_handler;
		Events* events;
		rRenderer* renderer;
	private:
		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		WindowProperties* properties;
	};

	class Button {
	public:
		Button(Events* events, const Rect& button);
		Button(Events* events, int x, int y, int w, int h);
		bool Hover();
		bool Click(ButtonIds id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);

		void UpdatePosition(IVec2& pos) { position.pos = pos; }
		void UpdateSize(IVec2 size) { position.size = size; }

		bool Hold(ButtonIds id);
		bool InOut(ButtonIds id);
		inline Rect Position() { return position; }
	protected:
		Rect position;
		Events* events;
		bool in;
		bool clicked;
	};

	struct GridComponents {
		int rows, cols;
		int block_width, block_height;
		int start_x, start_y;
	};

	class Grid {
	public:
		Grid(rRenderer* renderer, Events* Events, const GridComponents& grid);
		virtual void ResizeGrid(GridComponents& grid);
		void RenderRect(int col, int row, const Color& color);
		void RenderBorder(const Color& color);
		IVec2 Hover();
		IVec2 Click(ButtonIds id);
		IVec2 Hold(ButtonIds id);

		void Personalize(const std::function<void(int x, int y, int bw, int bh)>& function);

		IVec2 ConvertPoints(int col, int row);
		inline GridComponents GridComponent() const { return grid; }

		Events* Eventss() { return events; }
		rRenderer* Render() { return renderer; }

		void SetCameraPosition(const IVec2& camera_position) { this->camera_position = camera_position; }
	protected:
		GridComponents grid;
		int x, y;

		IVec2 camera_position;

		rRenderer* renderer;
		Events* events;
		Button button;

	};

	class SpriteSheet {
	public:
		SpriteSheet(Texture& texture, int rows, int cols);

		void SelectSprite(int x, int y);
		void DrawSelectedSprite(const Rect& rect);

		Rect ReturnSourceRect();

		IVec2 Size() { return { col, row }; }
	private:
		Rect clip;
		Texture texture;
		int row, col;
	};

	struct TileInfo {
		int texture_id;
	};

	class TileMapSerializer {
	public:
		TileMapSerializer(GridComponents& grid, const char* file_path);
		void Save(GridComponents& grid, std::vector<TileInfo>& info);

		void ReadTextureInfo(int col, int row, std::vector<TileInfo>& data);
		size_t ReadLayeredTexture(int col, int row, std::vector<std::vector<TileInfo>>& data);

		std::vector<TileInfo> AddNewLayer(int col, int row);

	private:
		File serialier;
		int index;
		std::stringstream output;

		int col_count, row_count;

		void ClearStream();
		void Read(int& value);
		void PullSize(GridComponents& grid);
		void PullGrid(GridComponents& grid);

		bool gc_saved;
	};

	class TileMap : public Grid {
	public:
		TileMap(rRenderer* renderer, Events* events, const GridComponents& grid);

		void AddLayer(std::vector<TileInfo>& texture_info);
		void RenderTextures(SpriteSheet& sprite_sheet);
		void UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num);
	private:
		std::vector<std::vector<TileInfo>> layers_texture_info;
	};

	class TileMapEditor {
	public:
		TileMapEditor(TileMap& map);
		void UpdateMap(std::vector<TileInfo>& texture_info, ButtonIds btn_id, const IVec2& camera_position);
		void RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, ButtonIds btn_id, const IVec2& size);
		void DeleteTile(std::vector<TileInfo>& texture_info, SDL_Scancode scancode);
	private:
		TileMap map;
		int current_user_texture;
		Button texture_picker;
	};
}

#endif // !EMBER_H

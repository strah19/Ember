#include "Ember.h"

namespace ember {
	void CheckVersion() {
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		printf("We compiled against SDL version %d.%d.%d ...\n",
			compiled.major, compiled.minor, compiled.patch);
		printf("But we are linking against SDL version %d.%d.%d.\n",
			linked.major, linked.minor, linked.patch);

		EmberAssert(linked.major == 2 || compiled.major == 2, "Version of SDL2 must be 2.0 or higher");
	}

	SDL_DisplayMode GetDisplay() {
		SDL_DisplayMode display_mode;

		if (SDL_GetDesktopDisplayMode(0, &display_mode) != 0)
		{
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return display_mode;
		}

		return display_mode;
	}

	EmberVec2::EmberVec2(const float x, const float y)
		:x(x), y(y) {
	}

	std::ostream& operator<<(std::ostream& os, const EmberVec2& vec2) {
		os << vec2.x << " " << vec2.y;
		return os;
	}

	EmberVec2 operator+(const EmberVec2& v1, const EmberVec2& v2) {
		return (EmberVec2(v1.x + v2.x, v1.y + v2.y));
	}

	EmberVec2 operator-(const EmberVec2& v1, const EmberVec2& v2) {
		return (EmberVec2(v1.x - v2.x, v1.y - v2.y));
	}

	EmberVec2 operator*(const EmberVec2& v1, const EmberVec2& v2) {
		return (EmberVec2(v1.x * v2.x, v1.y * v2.y));
	}

	EmberVec2 operator/(const EmberVec2& v1, const EmberVec2& v2) {
		return (EmberVec2(v1.x / v2.x, v1.y / v2.y));
	}

	EmberVec2 operator+(const EmberVec2& v1, const float s) {
		return (EmberVec2(v1.x + s, v1.y + s));
	}

	EmberVec2 operator-(const EmberVec2& v1, const float s) {
		return (EmberVec2(v1.x - s, v1.y - s));
	}

	EmberVec2 operator*(const EmberVec2& v1, const float s) {
		return (EmberVec2(v1.x * s, v1.y * s));
	}

	EmberVec2 operator/(const EmberVec2& v1, const float s) {
		return (EmberVec2(v1.x / s, v1.y / s));
	}

	void EmberVec2::operator+=(const float scalar) {
		this->x += scalar;
		this->y += scalar;
	}

	void EmberVec2::operator-=(const float scalar) {
		this->x -= scalar;
		this->y -= scalar;
	}

	void EmberVec2::operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	void EmberVec2::operator/=(const float scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void EmberVec2::operator+=(const EmberVec2& vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void EmberVec2::operator-=(const EmberVec2& vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void EmberVec2::operator*=(const EmberVec2& vec) {
		this->x *= vec.x;
		this->y *= vec.y;
	}

	void EmberVec2::operator/=(const EmberVec2& vec) {
		this->x /= vec.x;
		this->y /= vec.y;
	}

	void EmberVec2::Negate() {
		this->x *= -1;
		this->y *= -1;
	}

	float EmberVec2::Magnitude() {
		return (std::sqrt((this->x * this->x) + (this->y * this->y)));
	}

	EmberVec2 EmberVec2::Normalize() {
		EmberVec2 return_vec2 = { this->x, this->y };
		float m = Magnitude();
		return_vec2.x /= m;
		return_vec2.y /= m;

		return return_vec2;
	}

	bool operator!=(const EmberVec2& v1, const EmberVec2& v2) {
		if (v1.x != v2.x && v1.y != v2.y) {
			return true;
		}
		return false;
	}

	float EmberVec2::DotProduct(EmberVec2& vec2) {
		return ((this->x * vec2.x) + (this->y * vec2.y));
	}

	EmberVec4::EmberVec4(float x, float y, float w, float h)
		:a(x, y), b(w, h) {
	}
	EmberVec4::EmberVec4()
		: a(0, 0), b(0, 0) {
	}

	KeyBoardEvents::KeyBoardEvents()
		: pressed(false), released(false), repeat(0) {
	}

	MouseEvents::MouseEvents()
		: x(0), y(0), down(false), up(false), id(-1), clicks(0), state(0) {
	}

	MouseMotionEvents::MouseMotionEvents()
		: x_direction(0), y_direction(0) {
	}

	MouseWheelEvents::MouseWheelEvents()
		: x_direction(0), y_direction(0) {
	}

	JoyStickEvents::JoyStickEvents() 
		: down(false), up(false), button_id(0), value(0), axis_id(0) {
	}

	EventHandler::EventHandler()
		: closeWithEscape(false), resize(false), joystick_active(false) {
	}

	SDLProperties::SDLProperties()
		: window_title("EmberScreen"), window(NULL), renderer(NULL), event(), width(1000), height(800), full_screen(false),
		window_x_position(SDL_WINDOWPOS_UNDEFINED), window_y_position(SDL_WINDOWPOS_UNDEFINED) {
	}

	SDLProperties::SDLProperties(const char* title, int width, int height, bool full, int x, int y)
		: window_title(title), window(NULL), renderer(NULL), event(), width(width), height(height), full_screen(full),
		window_x_position(x), window_y_position(y) {
	}

	EmberScreen::EmberScreen(SDLProperties* properties)
		: m_properties(nullptr), m_latest(0), m_delta_time(0.0f), m_joy(nullptr) {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			Running = false;
			std::cout << "Could Not Initialize SDL_INIT" << std::endl;
			SDL_Quit();
		}
		else {
			int full_screen_flag = 0;
			if (properties->full_screen) {
				full_screen_flag = SDL_WINDOW_FULLSCREEN;
			}
			properties->window = SDL_CreateWindow(properties->window_title.data(), properties->window_x_position, properties->window_y_position, properties->width,
				properties->height, full_screen_flag);
			properties->renderer = SDL_CreateRenderer(properties->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (properties->renderer == NULL || properties->window == NULL) {
				Running = false;
				std::cout << "Could Not Properly Create A Window And Renderer" << std::endl;
				SDL_Quit();
			}
			else {
				Running = true;
				std::cout << "Loaded SDL2 And Created A Window And Renderer Called " << properties->window_title << std::endl;
			}

			if (SDL_NumJoysticks() < 1) {
				m_handle_events.joystick_active = false;
			} else {
				m_joy = SDL_JoystickOpen(0);
				if (m_joy == nullptr) {
					m_handle_events.joystick_active = false;
				}
				std::cout << "Joystick is initialized but possibly not active." << std::endl;
			}

			m_latest = SDL_GetTicks();
		}

		Running = InitializeAllAssets();

		this->m_properties = properties;
		EmberAssert(this->m_properties != NULL, "Could not initialize properties of ember screen");
	}

	void EmberScreen::StartTime() {
		m_delta_time = (float) (SDL_GetTicks() - m_latest);
		m_latest = SDL_GetTicks();
	}

	bool EmberScreen::IterateEvents() {
		while (SDL_PollEvent(&m_properties->event)) {
			switch (m_properties->event.type) {
			case SDL_KEYDOWN: {
				m_handle_events.keyBoard.name = SDL_GetKeyName(m_properties->event.key.keysym.sym);
				m_handle_events.keyBoard.pressed = true;
				m_handle_events.keyBoard.released = false;
				m_handle_events.keyBoard.repeat = m_properties->event.key.repeat;
				if (m_handle_events.closeWithEscape) {
					switch (m_properties->event.key.keysym.sym) {
					case SDLK_ESCAPE: {
						return false;
						break;
					}
					}
				}
				break;
			}
			case SDL_KEYUP: {
				m_handle_events.keyBoard.pressed = false;
				m_handle_events.keyBoard.released = true;
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				m_handle_events.mouse.down = true;
				m_handle_events.mouse.up = false;
				m_handle_events.mouse.id = m_properties->event.button.button;
				m_handle_events.mouse.clicks = m_properties->event.button.clicks;
				m_handle_events.mouse.state = SDL_PRESSED;
				break;
			}
			case SDL_MOUSEBUTTONUP: {
				m_handle_events.mouse.down = false;
				m_handle_events.mouse.up = true;
				m_handle_events.mouse.state = SDL_RELEASED;
				break;
			}
			case SDL_MOUSEMOTION: {
				m_handle_events.motion.x_direction = m_properties->event.motion.x;
				m_handle_events.motion.y_direction = m_properties->event.motion.y;
				break;
			}
			case SDL_MOUSEWHEEL: {
				m_handle_events.wheel.x_direction = m_properties->event.wheel.x;
				m_handle_events.wheel.y_direction = m_properties->event.wheel.y;		
				break;
			}
			case SDL_QUIT: {
				return false;
				break;
			}
			case SDL_WINDOWEVENT: {
				switch (m_properties->event.window.event) {
				case SDL_WINDOWEVENT_RESIZED: {
					m_properties->width = m_properties->event.window.data1;
					m_properties->height = m_properties->event.window.data2;
					break;
				}
				case SDL_WINDOWEVENT_MOVED: {
					m_properties->window_x_position = m_properties->event.window.data1;
					m_properties->window_y_position = m_properties->event.window.data2;
					break;
				}
				}
			}
			case SDL_JOYBUTTONDOWN: {
				if (m_handle_events.joystick_active && m_joy != nullptr) {
					m_handle_events.joystick.down = true;
					m_handle_events.joystick.up = false;
					m_handle_events.joystick.button_id = m_properties->event.jbutton.button;
				}
				break;
			}
			case SDL_JOYBUTTONUP: {
				if (m_handle_events.joystick_active && m_joy != nullptr) {
					m_handle_events.joystick.down = false;
					m_handle_events.joystick.up = true;
				}
				break;
			}
			case SDL_JOYAXISMOTION: {
				if (m_handle_events.joystick_active && m_joy != nullptr) {
					m_handle_events.joystick.axis_id = m_properties->event.jaxis.axis;
					m_handle_events.joystick.value = m_properties->event.jaxis.value;
				}
				break;
			}
			}
		}

		m_handle_events.mouse.x = m_properties->event.button.x;
		m_handle_events.mouse.y = m_properties->event.button.y;

		if (m_handle_events.resize) {
			SDL_SetWindowResizable(m_properties->window, SDL_TRUE);
		}

		return true;
	}

	bool EmberScreen::KeyboardState(SDL_Scancode scancode) {
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[scancode]) {
			return true;
		}
		return false;
	}

	void EmberScreen::AddEventFlags(EventFlags flag) {
		if (flag == EventFlags::kResize) {
			m_handle_events.resize = true;
		}
		else if (flag == EventFlags::kEscape) {
			m_handle_events.closeWithEscape = true;
		}
		else if (flag == EventFlags::kJoystick) {
			m_handle_events.joystick_active = true;
		}
	}

	void EmberScreen::Close() {
		SDL_DestroyWindow(m_properties->window);
		SDL_DestroyRenderer(m_properties->renderer);
		SDL_JoystickClose(m_joy);

		std::cout << "Closing And Cleaning Ember/SDL2" << std::endl;

		m_properties = nullptr;

		IMG_Quit();
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();

	}

	EmberScreen::~EmberScreen() {
		Close();
	}

	Cursor::Cursor() {
		m_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	}

	Cursor::~Cursor() {
		SDL_FreeCursor(m_cursor);
	}

	void Cursor::SetCursor(SDL_SystemCursor cursor_type) {
		m_cursor = SDL_CreateSystemCursor(cursor_type);
	}

	void Cursor::Update()
	{
		SDL_SetCursor(m_cursor);
	}

	void Cursor::ShowCursor() {
		SDL_ShowCursor(SDL_ENABLE);
	}

	void Cursor::HideCursor() {
		SDL_ShowCursor(SDL_DISABLE);
	}

	bool InitializeFonts() {
		if (TTF_Init() == -1) {
			return false;
		}
		std::cout << "Initialized SDL_ttf" << std::endl;
		return true;
	}

	bool InitializeMixer() {
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		std::cout << "Initialized SDL_Mixer" << std::endl;
		return true;
	}

	bool InitializeImg() {
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags)) {
			return false;
		}
		std::cout << "Initialized SDL_image" << std::endl;
		return true;
	}

	bool InitializeAllAssets() {
		return (InitializeFonts() && InitializeImg() && InitializeMixer());
	}

	Font::Font(EmberScreen* screen, const char* file_path, const char* text, int size, const SDL_Color& color, int x, int y)
		: m_screen(screen), m_font_width(0), m_font_height(0), m_font(TTF_OpenFont(file_path, size)), m_font_position({ x, y, 0, 0 }),
		m_font_texture(NULL), m_font_color(color), m_font_is_locked(true) {
		if (!m_font) {
			std::cout << "Could Not Load Font Data From: " << file_path << std::endl;
			screen->Close();
		}
		else {
			std::cout << "Loaded Font Data From: " << file_path << std::endl;
		}

		UnlockFont();
		ChangeFont(text, color);
		LockFont();
	}

	void Font::UnlockFont() {
		m_font_is_locked = false;
	}

	void Font::LockFont() {
		m_font_is_locked = true;
	}

	void Font::Render() {
		if (!m_font_is_locked) {
			SDL_RenderCopy(m_screen->Properties()->renderer, m_font_texture, NULL, &m_font_position);
		}
	}

	void Font::SetPosition(int x, int y) {
		if (!m_font_is_locked) {
			m_font_position.x = x;
			m_font_position.y = y;
		}
	}

	void Font::Update(int x, int y) {
		if (!m_font_is_locked) {
			m_font_position.x = m_font_position.x + x;
			m_font_position.y = m_font_position.y + y;
		}
	}

	void Font::CleanUp() {
		TTF_CloseFont(m_font);
	}


	Image::Image(EmberScreen* screen, const char* file_path, int x, int y, int w, int h)
		: m_screen(screen), m_texture(LoadTexture(screen, file_path)), m_position({ x, y, w, h }) {
		std::cout << "Created Image: " << file_path << std::endl;
	}

	Image::Image(EmberScreen* screen, const char* file_path, SDL_Rect& position)
		: m_screen(screen), m_texture(LoadTexture(screen, file_path)), m_position(position) {
		std::cout << "Created Image: " << file_path << std::endl;
	}

	void Image::Render(float angle, SDL_RendererFlip flip) {
		Draw(m_screen, m_texture, m_position, angle, flip);
	}

	void Image::SetImageAlptha(int alptha) {
		TextureAlptha(m_texture, alptha);
	}

	void Image::SetColor(int r, int g, int b) {
		TextureColor(m_texture, r, g, b);
	}

	void Image::SetPosition(int x, int y) {
		m_position.x = x;
		m_position.y = y;
	}

	Image::~Image() {
		DeleteTexture(m_texture);
		std::cout << "Deleted Image" << std::endl;
	}

	AudioChunk::AudioChunk(const char* file_path)
		: m_chunk(Mix_LoadWAV(file_path)), m_volume(0) {
		if (m_chunk == NULL)
			std::cout << "Could Not Load Chunk: " << file_path << std::endl;
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
		if (m_music == NULL)
			std::cout << "Could Not Load Music: " << file_path << std::endl;
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

	SDL_Texture* LoadTexture(EmberScreen* screen, const char* file_path) {
		if (file_path == NULL) {
			return NULL;
		}

		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(file_path);

		if (Surface == NULL) {
			std::cout << "Could Not Load Texture: " << file_path << std::endl;
			screen->Close();
		}
		else {
			std::cout << "Loaded Texture: " << file_path << std::endl;
		}

		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(screen->Properties()->renderer, Surface);
		SDL_FreeSurface(Surface);

		return mainTexture;
	}

	SDL_Texture* CreateTextureFromSurface(EmberScreen* screen, SDL_Surface* surface) {
		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(screen->Properties()->renderer, surface);
		SDL_FreeSurface(surface);

		std::cout << "Created Texture From Surface" << std::endl;

		return mainTexture;
	}

	SDL_Surface* LoadSurface(const char* file_path) {
		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(file_path);
		if (Surface == NULL) {
			std::cout << "Could Not Load Surface: " << file_path << std::endl;
		}
		else {
			std::cout << "Loaded Surface: " << file_path << std::endl;
		}

		return Surface;
	}

	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyExF(screen->Properties()->renderer, texture, NULL, &dest, angle, NULL, flip);
	}

	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyExF(screen->Properties()->renderer, texture, &src, &dest, angle, NULL, flip);
	}

	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyEx(screen->Properties()->renderer, texture, NULL, &dest, angle, NULL, flip);
	}

	void Draw(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyEx(screen->Properties()->renderer, texture, &src, &dest, angle, NULL, flip);
	}

	void SetWindowIcon(EmberScreen* screen, const char* filePath) {
		SDL_Surface* icon = NULL;
		icon = IMG_Load(filePath);
		if (icon == NULL) {
			std::cout << "Could Not Load Window Icon Surface:  " << filePath << std::endl;
			screen->Close();
		}
		else {
			std::cout << "Loaded Window Icon Surfcace: " << filePath << std::endl;
		}
		SDL_SetWindowIcon(screen->Properties()->window, icon);
		SDL_FreeSurface(icon);
	}

	void DeleteTexture(SDL_Texture* texture) {
		SDL_DestroyTexture(texture);
	}

	void TextureColor(SDL_Texture* texture, int r, int g, int b) {
		SDL_SetTextureColorMod(texture, r, g, b);
	}

	void TextureAlptha(SDL_Texture* texture, int a) {
		SDL_SetTextureAlphaMod(texture, a);
	}

	void GetTextureInfo(SDL_Texture* texture, int& w, int& h) {
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	}

	void Pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawPoint(renderer, x, y);
	}

	void Pixel(SDL_Renderer* renderer, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawPointF(renderer, x, y);
	}

	void HorizontalLine(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y1);
	}

	void VerticalLine(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x1, y2);
	}

	void Line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	void HorizontalLine(SDL_Renderer* renderer, float x1, float x2, float y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLineF(renderer, x1, y1, x2, y1);
	}

	void Line(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	}

	void VerticalLine(SDL_Renderer* renderer, float y1, float y2, float x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLineF(renderer, x1, y1, x1, y2);
	}

	void RectangleBorder(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_Rect Position = { x, y, w, h };
		SDL_RenderDrawRect(renderer, &Position);

	}

	void RectangleFill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_Rect Position = { x, y, w, h };
		SDL_RenderFillRect(renderer, &Position);

	}

	void RectangleBorder(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_FRect Position = { x, y, w, h };
		SDL_RenderDrawRectF(renderer, &Position);

	}

	void RectangleFill(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_FRect Position = { x, y, w, h };
		SDL_RenderFillRectF(renderer, &Position);

	}

	void FillCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		for (double dy = 1; dy <= radius; dy += 1.0)
		{
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			int x = centreX - (int)dx;
			SDL_RenderDrawLine(renderer, centreX - (int)dx, centreY + (int)dy - radius, centreX + (int)dx, centreY + (int)dy - radius);
			SDL_RenderDrawLine(renderer, centreX - (int)dx, centreY - (int)dy + radius, centreX + (int)dx, centreY - (int)dy + radius);
		}
	}

	void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		const int diameter = (radius * 2);

		int x = (radius - 1);
		int y = 0;
		int tx = 1;
		int ty = 1;
		int error = (tx - diameter);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		while (x >= y) {
			SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
			SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
			SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

			if (error <= 0) {
				++y;
				error += ty;
				ty += 2;
			}

			if (error > 0) {
				--x;
				tx += 2;
				error += (tx - diameter);
			}
		}
	}

	bool PointVsRect(const EmberVec2& point, const EmberRect& rect) {
		return (point.x >= rect.pos.x && point.y >= rect.pos.y && point.x < rect.pos.x + rect.pos.w && point.y < rect.pos.y + rect.pos.h);
	}

	bool RectVsRect(const EmberRect& rect1, const EmberRect& rect2) {
		return (rect1.pos.x < rect2.pos.x + rect2.pos.w && rect1.pos.x + rect1.pos.w > rect2.pos.x &&
			rect1.pos.y < rect2.pos.y + rect2.pos.h && rect1.pos.y + rect1.pos.h > rect2.pos.y);
	}

	bool RayVsRect(const EmberVec2& ray, const EmberVec2& ray_dir, const EmberRect& rect, EmberVec2& contact, EmberVec2& contact_normal, float& hit_near) {
		EmberVec2 near = (rect.pos.a - ray) / ray_dir;
		EmberVec2 far = (rect.pos.a + rect.pos.b - ray) / ray_dir;

		if (near.x > far.x)
			std::swap(near.x, far.x);
		if (near.y > far.y)
			std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x)
			return false;

		hit_near = std::max(near.x, near.y);
		float hitFar = std::min(far.x, far.y);

		if (hitFar < 0)
			return false;

		contact.x = ray.x + hit_near * ray_dir.x;
		contact.y = ray.y + hit_near * ray_dir.y;

		if (near.x > near.y)
		{
			if (ray_dir.x < 0)
			{
				contact_normal = { 1, 0 };
			}
			else
			{
				contact_normal = { -1, 0 };
			}
		}
		else if (near.x < near.y)
		{
			if (ray_dir.y < 0)
			{
				contact_normal = { 0, 1 };
			}
			else
			{
				contact_normal = { 0, -1 };
			}
		}

		return true;
	}

	bool DynamicRectVsRect(const EmberRect& rect1, const EmberRect& rect2, EmberVec2& contact, EmberVec2& contact_normal, float& hit_near) {
		if (rect1.vel.x == 0 && rect1.vel.y == 0)
			return false;

		EmberRect expand;
		expand.pos.a = rect2.pos.a - rect1.pos.b / 2 + (rect1.pos.b / 2 + 1);
		expand.pos.b = rect2.pos.b + rect1.pos.b - 2;

		if (RayVsRect(rect1.pos.a + rect1.pos.b, rect1.vel, expand, contact, contact_normal, hit_near))
		{
			if (hit_near <= 1.0f)
			{
				return true;
			}
		}

		return false;
	}

	void BeganRender(EmberScreen* screen, Uint8 r, Uint8 g, Uint8 b) {
		SDL_SetRenderDrawColor(screen->Properties()->renderer, r, g, b, 255);
		SDL_RenderClear(screen->Properties()->renderer);
	}

	void CloseRender(EmberScreen* screen) {
		SDL_RenderPresent(screen->Properties()->renderer);
	}

	void Shadow(SDL_Renderer* renderer, SDL_Rect& position, int depth, SDL_Color start_color, int color_change)
	{
		if ((depth - 1) * color_change + start_color.a > 0) {
			ember::RectangleFill(renderer, position.x, position.y, position.w, position.h, start_color.r, start_color.g, start_color.b, start_color.a);
			for (int i = 0; i < depth; i++) {
				ember::RectangleFill(renderer, position.x, position.y + position.h + i, position.w, 1, start_color.r, start_color.g, start_color.b, start_color.a - (i * color_change));
			}
		}
	}

	void Curve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color) {
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
			float px = ((1 - t) * (1 - t)) * x[0] + 2 * (1 - t) * t * x[1] + (t * t) * x[2];
			float py = ((1 - t) * (1 - t)) * y[0] + 2 * (1 - t) * t * y[1] + (t * t) * y[2];
			ember::Pixel(renderer, px, py, color.r, color.g, color.b, color.a);
		}
	}

	void AdvCurve(SDL_Renderer* renderer, int x[], int y[], SDL_Color color) {
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
			float px = ((1 - t) * (1 - t) * (1 - t)) * x[0] + 3 * ((1 - t) * (1 - t)) * t * x[1] + 3 * (1 - t) * (t * t) * x[2] + (t * t * t) * x[3];
			float py = ((1 - t) * (1 - t) * (1 - t)) * y[0] + 3 * ((1 - t) * (1 - t)) * t * y[1] + 3 * (1 - t) * (t * t) * y[2] + (t * t * t) * y[3];
			ember::Pixel(renderer, px, py, color.r, color.g, color.b, color.a);
		}
	}

	bool PointToCircle(EmberVec2& point, Circle& circle) {
		int dx = (int)abs((int)point.x - circle.x);
		int dy = (int)abs((int)point.y - circle.y);

		if (dx > circle.radius || dy > circle.radius) {
			return false;
		}

		if (dx + dy <= circle.radius) {
			return true;
		}

		if (dx * dx + dy * dy <= circle.radius * circle.radius) {
			return true;
		}
		else {
			return false;
		}
	}

	bool PointToCircle(EmberVec2& point, CircleF& circle) {
		float dx = (float)abs(point.x - circle.x);
		float dy = (float)abs(point.y - circle.y);

		if (dx > circle.radius || dy > circle.radius) {
			return false;
		}

		if (dx + dy <= circle.radius) {
			return true;
		}

		if (dx * dx + dy * dy <= circle.radius * circle.radius) {
			return true;
		}
		else {
			return false;
		}
	}

	Button::Button(EmberScreen* screen, SDL_Rect& button)
		:m_screen(screen), m_position(button), m_in(false), m_clicked(false) {
	}

	Button::Button(EmberScreen* screen, int x, int y, int w, int h)
		: m_screen(screen), m_position({ x, y, w, h }), m_in(false), m_clicked(false) {
	}

	bool Button::Hover() {
		if (m_screen->Events()->mouse.x >= m_position.x && m_screen->Events()->mouse.x <= m_position.x + m_position.w && m_screen->Events()->mouse.y >= m_position.y
			&& m_screen->Events()->mouse.y <= m_position.y + m_position.h) {
			return true;
		}
		return false;
	}

	bool Button::Click(int id) {
		if (m_screen->Events()->mouse.down && m_screen->Events()->mouse.id == id && !m_clicked) {
			if (m_screen->Events()->mouse.x >= m_position.x && m_screen->Events()->mouse.x <= m_position.x + m_position.w && m_screen->Events()->mouse.y >= m_position.y
				&& m_screen->Events()->mouse.y <= m_position.y + m_position.h) {
				m_clicked = true;
				return true;
			}
		}
		if (m_clicked) {
			if (m_screen->Events()->mouse.up) {
				m_clicked = false;
			}
		}
		return false;
	}

	void Button::UpdatePosition(int x, int y) {
		m_position.x = x;
		m_position.y = y;
	}

	void Button::UpdateSize(int w, int h) {
		m_position.w = w;
		m_position.h = h;
	}

	bool Button::InOut(int id) {
		bool click = Click(id);
		if (click && !m_in) {
			m_in = true;
		}
		else if (click && m_in) {
			m_in = false;
		}

		return m_in;
	}

	bool Button::Hold(int id) {
		if (m_screen->Events()->mouse.down && m_screen->Events()->mouse.id == id) {
			if (m_screen->Events()->mouse.x >= m_position.x && m_screen->Events()->mouse.x <= m_position.x + m_position.w
				&& m_screen->Events()->mouse.y >= m_position.y && m_screen->Events()->mouse.y <= m_position.y + m_position.h) {
				return true;
			}
		}
		return false;
	}

	Grid::Grid(EmberScreen* screen, int x, int y, unsigned int col, unsigned int row, int width_size, int height_size)
		:m_screen(screen), m_rows(row), m_cols(col), m_block_width(width_size), m_block_height(height_size),
		m_x(x), m_y(y), m_start_x(x), m_start_y(y), m_button(screen, x, y, m_block_width, m_block_height) {
	}

	void Grid::ResizeGrid(int x, int y, unsigned int col, unsigned int row, int width_size, int height_size) {
		m_start_x = x;
		m_start_y = y;

		m_block_width = width_size;
		m_block_height = height_size;

		m_cols = col;
		m_rows = row;
	}

	void Grid::RenderRect(unsigned int col, unsigned int row, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		RectangleFill(m_screen->Properties()->renderer, m_start_x + col * m_block_width, m_start_y + row * m_block_height, m_block_width, m_block_height, r, g, b, a);
	}

	void Grid::RenderBorder(Uint8 r, Uint8 g, Uint8 b) {
		m_x = m_start_x;
		m_y = m_start_y;

		for (unsigned int i = 0; i < m_cols; i++) {
			for (unsigned int j = 0; j < m_rows; j++) {
				HorizontalLine(m_screen->Properties()->renderer, m_x, m_x + m_block_width, m_y, r, g, b);
				VerticalLine(m_screen->Properties()->renderer, m_y, m_y + m_block_height, m_x, r, g, b);

				m_y += m_block_height;
			}
			m_x += m_block_width;
			m_y = m_start_y;
		}
		HorizontalLine(m_screen->Properties()->renderer, m_start_x, m_start_x + (m_block_width * m_cols), m_start_y + (m_block_height * m_rows), r, g, b);
		VerticalLine(m_screen->Properties()->renderer, m_start_y, m_start_y + (m_block_height * m_rows), m_start_x + (m_block_width * m_cols), r, g, b);
	}

	EmberVec2 Grid::Hover() {
		m_x = m_start_x;
		m_y = m_start_y;

		for (unsigned int i = 0; i < m_cols; i++) {
			for (unsigned int j = 0; j < m_rows; j++) {
				m_button.UpdatePosition(m_x, m_y);
				if (m_button.Hover()) {
					return { (float)i, (float)j };
				}

				m_y += m_block_height;
			}
			m_x += m_block_width;
			m_y = m_start_y;
		}
		return { -1.0f, -1.0f };
	}

	EmberVec2 Grid::Click(int id) {
		m_x = m_start_x;
		m_y = m_start_y;

		for (unsigned int i = 0; i < m_cols; i++) {
			for (unsigned int j = 0; j < m_rows; j++) {
				m_button.UpdatePosition(m_x, m_y);
				if (m_button.Click(id)) {
					return { (float)i, (float)j };
				}
				m_y += m_block_height;
			}
			m_x += m_block_width;
			m_y = m_start_y;
		}
		return { -1.0f, -1.0f };
	}

	EmberVec2 Grid::Hold(int id) {
		m_x = m_start_x;
		m_y = m_start_y;

		for (unsigned int i = 0; i < m_cols; i++) {
			for (unsigned int j = 0; j < m_rows; j++) {
				m_button.UpdatePosition(m_x, m_y);
				if (m_button.Hold(id)) {
					return { (float)i, (float)j };
				}
				m_y += m_block_height;
			}
			m_x += m_block_width;
			m_y = m_start_y;
		}
		return { -1.0f, -1.0f };
	}

	EmberVec2 Grid::ConvertPoints(unsigned int col, unsigned int row) {
		return { m_start_x + ((float)col * m_block_width), m_start_y + ((float)row * m_block_height) };
	}

	SpriteSheet::SpriteSheet(EmberScreen* screen, char const* file_path, int row, int column) {
		m_screen = screen;
		m_sheet = LoadTexture(m_screen, file_path);

		SDL_QueryTexture(m_sheet, NULL, NULL, &m_clip.w, &m_clip.h);

		m_clip.w /= column;
		m_clip.h /= row;

		m_row = row;
		m_col = column;
		
	}

	SpriteSheet::~SpriteSheet() {
		DeleteTexture(m_sheet);
	}

	void SpriteSheet::SelectSprite(int x, int y) {
		m_clip.x = x * m_clip.w;
		m_clip.y = y * m_clip.h;
	}

	void SpriteSheet::DrawSelectedSprite(SDL_Rect& position) {
		SDL_RenderCopy(m_screen->Properties()->renderer, m_sheet, &m_clip, &position);
	}

	Camera::Camera(EmberScreen* screen)
		:m_screen(screen), m_offset_x(0.0f), m_offset_y(0.0f), m_scale_x(1.0f), m_scale_y(1.0f), m_down_flag(true),
		 m_pan_x(0.0f), m_pan_y(0.0f) {
	}

	Camera::Camera(EmberScreen* screen, float offset_x, float offset_y)
		:m_screen(screen), m_offset_x(offset_x), m_offset_y(offset_y), m_scale_x(1.0f), m_scale_y(1.0f), m_down_flag(true),
		 m_pan_x(offset_x), m_pan_y(offset_y) {
	}

	void Camera::Pan(int button_click) {
		if (m_screen->Events()->mouse.down && m_down_flag && m_screen->Events()->mouse.id == button_click) {
			m_pan_x = (float) m_screen->Events()->mouse.x;
			m_pan_y = (float)m_screen->Events()->mouse.y;
			m_down_flag = false;
		}
		else if (m_screen->Events()->mouse.up) {
			m_down_flag = true;
		}

		if (m_screen->Events()->mouse.down && m_screen->Events()->mouse.id == button_click) {
			m_offset_x -= ((float) m_screen->Events()->mouse.x - m_pan_x) / m_scale_x;
			m_offset_y -= ((float) m_screen->Events()->mouse.y - m_pan_y) / m_scale_y;

			m_pan_x = (float) m_screen->Events()->mouse.x;
			m_pan_y = (float) m_screen->Events()->mouse.y;
		}
	} 

	void Camera::Scale(float scale_x, float scale_y) {
		m_scale_x *= scale_x;
		m_scale_y *= scale_y;
	}

	void Camera::MoveCameraOffset(float offset_x, float offset_y) {
		m_offset_x = -offset_x;
		m_offset_y = -offset_y;  
	}

	void Camera::WorldToScreen(float f_world_x, float f_world_y, int& screen_x, int& screen_y) {
		screen_x = (int)((f_world_x - m_offset_x) * m_scale_x);
		screen_y = (int)((f_world_y - m_offset_y) * m_scale_y);
	}

	void Camera::ScreenToWorld(int screen_x, int screen_y, float& f_world_x, float& f_world_y) {
		f_world_x = (float)((screen_x / m_scale_y) + m_offset_x);
		f_world_y = (float)((screen_y / m_scale_y) + m_offset_y);
	}

	void Camera::RenderBoundry() {
		VerticalLine(m_screen->Properties()->renderer, m_offset_y, (float) m_screen->Properties()->height, m_offset_x , 255, 255, 255);
		HorizontalLine(m_screen->Properties()->renderer, m_offset_x, (float)m_screen->Properties()->width, m_offset_y, 255, 255, 255);
	}

	Timer::Timer() 
		: m_current_time(0), m_last_time(0), m_second_pass(0) {

	}

	int Timer::StartStopWatch()
	{
		m_current_time = SDL_GetTicks();
		if (m_current_time > m_last_time + 1000) {
			m_last_time = m_current_time;
			m_second_pass++;
		}
		return m_second_pass;
	}

	bool Timer::CreateTimer(unsigned int time)
	{
		if (m_second_pass == time) {
			return true;
		}
		else
			return false;
	}
}

#include "Ember.h"

namespace Ember
{
	void CheckVersion()
	{
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

	SDL_DisplayMode GetDisplay()
	{
		SDL_DisplayMode dm;

		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
			return dm;
		}

		return dm;
	}

	SDLProperties::SDLProperties()
		: m_WindowTitle("EmberScreen"), m_Window(NULL), m_Renderer(NULL), m_Event(), m_Width(1000), m_Height(800), m_FullScreen(false), 
		m_WindowPositionX(SDL_WINDOWPOS_UNDEFINED), m_WindowPositionY(SDL_WINDOWPOS_UNDEFINED)
	{
	}

	SDLProperties::SDLProperties(const char* title, unsigned int width, unsigned int height, bool full, int x, int y)
		: m_WindowTitle(title), m_Window(NULL), m_Renderer(NULL), m_Event(), m_Width(width), m_Height(height), m_FullScreen(full), m_WindowPositionX(x), m_WindowPositionY(y)
	{
	}

	EmberScreen::EmberScreen(SDLProperties* properties)
		:m_Properties(nullptr)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { //init SDL2
			m_IsRunning = false;
			std::cout << "Could Not Initialize SDL_INIT" << std::endl;
			SDL_Quit();
		}
		else {
			int flag = 0;
			if (properties->m_FullScreen)
				flag = SDL_WINDOW_FULLSCREEN;
			properties->m_Window = SDL_CreateWindow(properties->m_WindowTitle.data(), properties->m_WindowPositionX, properties->m_WindowPositionY, properties->m_Width
				, properties->m_Height, flag); //creates window
			properties->m_Renderer = SDL_CreateRenderer(properties->m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //creates renderer

			if (properties->m_Renderer == NULL || properties->m_Window == NULL) { //check that renderer has been created properly
				m_IsRunning = false;
				std::cout << "Could Not Properly Create A Window And Renderer" << std::endl;
				SDL_Quit();
			}
			else {
				m_IsRunning = true;
				std::cout << "Loaded SDL2 And Created A Window And Renderer Called " << properties->m_WindowTitle << std::endl;
			}
		}

		this->m_Properties = properties;
		EmberAssert(this->m_Properties != NULL, "Could not initialize properties of ember screen");
	}

	bool EmberScreen::IterateEvents()
	{
		switch (m_Properties->m_Event.type)
		{
		case SDL_KEYDOWN:
		{
			m_HandleEvents.keyBoard.name = SDL_GetKeyName(m_Properties->m_Event.key.keysym.sym);
			m_HandleEvents.keyBoard.pressed = true;
			m_HandleEvents.keyBoard.released = false;
			m_HandleEvents.keyBoard.repeat = m_Properties->m_Event.key.repeat;
			if (m_HandleEvents.closeWithEscape)
			{
				switch (m_Properties->m_Event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				{
					return false;
					break;
				}
				}
			}
		}
		break;
		case SDL_KEYUP:
		{
			m_HandleEvents.keyBoard.pressed = false;
			m_HandleEvents.keyBoard.released = true;
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			m_HandleEvents.mouse.down = true;
			m_HandleEvents.mouse.up = false;
			m_HandleEvents.mouse.id = m_Properties->m_Event.button.button;
			m_HandleEvents.mouse.clicks = m_Properties->m_Event.button.clicks;
		}
		break;
		case SDL_MOUSEBUTTONUP:
		{
			m_HandleEvents.mouse.down = false;
			m_HandleEvents.mouse.up = true;
		}
		break;
		case SDL_MOUSEMOTION:
		{
			m_HandleEvents.motion.yDirection = m_Properties->m_Event.motion.x;
			m_HandleEvents.motion.yDirection = m_Properties->m_Event.motion.y;
		}
		break;
		case SDL_MOUSEWHEEL:
		{
			m_HandleEvents.wheel.xDirection = m_Properties->m_Event.wheel.x;
			m_HandleEvents.wheel.yDirection = m_Properties->m_Event.wheel.y;
		}
		break;
		case SDL_QUIT:
		{
			return false;
		}
		break;
		case SDL_WINDOWEVENT:
		{
			switch (m_Properties->m_Event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			{
				m_Properties->m_Width = m_Properties->m_Event.window.data1;
				m_Properties->m_Height = m_Properties->m_Event.window.data2;
			}
			break;
			case SDL_WINDOWEVENT_MOVED:
			{
				m_Properties->m_WindowPositionX = m_Properties->m_Event.window.data1;
				m_Properties->m_WindowPositionY = m_Properties->m_Event.window.data2;
			}
			}
			break;
		}
		default:
			break;
		}

		m_HandleEvents.mouse.x = m_Properties->m_Event.button.x;
		m_HandleEvents.mouse.y = m_Properties->m_Event.button.y;

		if (m_HandleEvents.resize)
		{
			SDL_SetWindowResizable(m_Properties->m_Window, SDL_TRUE);
		}

		return true;
	}

	void EmberScreen::AddEventFlags(EventFlags flag)
	{
		if (flag == EventFlags::resize)
			m_HandleEvents.resize = true;
		else if (flag == EventFlags::escape)
			m_HandleEvents.closeWithEscape = true;
	}

	void EmberScreen::Close()
	{
		SDL_DestroyWindow(m_Properties->m_Window);
		SDL_DestroyRenderer(m_Properties->m_Renderer);

		std::cout << "Closing And Cleaning Ember/SDL2" << std::endl;

		m_Properties = nullptr;

		IMG_Quit();
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();

	}

	EmberScreen::~EmberScreen()
	{
		Close();
	}

	Cursor::Cursor()
	{
		m_Cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	}
	Cursor::~Cursor()
	{
		SDL_FreeCursor(m_Cursor);
	}
	void Cursor::set_cursor(SDL_SystemCursor type)
	{
		m_Cursor = SDL_CreateSystemCursor(type);
	}
	void Cursor::update()
	{
		SDL_SetCursor(m_Cursor);
	}
	void Cursor::show_cursor()
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	void Cursor::hide_cursor()
	{
		SDL_ShowCursor(SDL_DISABLE);
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
		return true;
	}

	bool initialize_img()
	{
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;
		std::cout << "Initialized SDL_image" << std::endl;
		return true;
	}

	bool initialize_all_asset_loaders()
	{
		return (initialize_font() && initialize_img() && initialize_mixer());
	}

	Font::Font(EmberScreen* screen, const char* filePath, const char* text, int size, const SDL_Color& color, int x, int y)
		: m_CurrentScreen(screen), m_FontWidth(0), m_FontHeight(0), m_FontData(TTF_OpenFont(filePath, size)), m_FontPosition({ x, y, 0, 0 }),
		m_FontTexture(NULL), m_FontColor(color), m_FontIsLocked(true)
	{
		if (!m_FontData)
		{
			std::cout << "Could Not Load Font Data From: " << filePath << std::endl;
			screen->Close();
		}
		else
		{
			std::cout << "Loaded Font Data From: " << filePath << std::endl;
		}

		unlock_font();
		change_font(text, color);
		lock_font();
	}

	void Font::unlock_font()
	{
		m_FontIsLocked = false;
	}

	void Font::change_font(const char* text, const SDL_Color& color)
	{
		if (!m_FontIsLocked) {
			m_FontColor = color;
			SDL_DestroyTexture(m_FontTexture);
			SDL_Surface* fontSurface = TTF_RenderText_Blended(m_FontData, text, m_FontColor);
			m_FontWidth = fontSurface->w;
			m_FontHeight = fontSurface->h;

			m_FontPosition.w = m_FontWidth;
			m_FontPosition.h = m_FontHeight;

			m_FontTexture = SDL_CreateTextureFromSurface(m_CurrentScreen->properties()->m_Renderer, fontSurface);

			SDL_FreeSurface(fontSurface);
		}
	}

	void Font::lock_font()
	{
		m_FontIsLocked = true;
	}

	void Font::render()
	{
		if (!m_FontIsLocked)
			SDL_RenderCopy(m_CurrentScreen->properties()->m_Renderer, m_FontTexture, NULL, &m_FontPosition);
	}

	void Font::set_position(int x, int y)
	{
		if (!m_FontIsLocked) {
			m_FontPosition.x = x;
			m_FontPosition.y = y;
		}
	}

	void Font::update(int x, int y)
	{
		if (!m_FontIsLocked) {
			m_FontPosition.x = m_FontPosition.x + x;
			m_FontPosition.y = m_FontPosition.y + y;
		}
	}

	void Font::clean_up()
	{
		TTF_CloseFont(m_FontData);
	}

	Image::Image(EmberScreen* screen, const char* filePath, int x, int y, int w, int h)
		: m_CurrentScreen(screen), m_Texture(load_texture(screen, filePath)), m_Position({ x, y, w, h })
	{
		std::cout << "Created Image: " << filePath << std::endl;
	}

	Image::Image(EmberScreen* screen, const char* filePath, SDL_Rect position)
		: m_CurrentScreen(screen), m_Texture(load_texture(screen, filePath)), m_Position(position)
	{
		std::cout << "Created Image: " << filePath << std::endl;
	}

	void Image::render(float angle, SDL_RendererFlip flip)
	{
		draw_no_source(m_CurrentScreen, m_Texture, m_Position, angle, flip);
	}

	void Image::set_image_alptha(int alpha)
	{
		set_alptha(m_Texture, alpha);
	}

	void Image::set_color(int r, int g, int b)
	{
		set_texture_color(m_Texture, r, g, b);
	}

	void Image::set_position(int x, int y)
	{
		m_Position.x = x;
		m_Position.y = y;
	}

	Image::~Image()
	{
		delete_texture(m_Texture);
		std::cout << "Deleted Image" << std::endl;
	}

	SDL_Texture* load_texture(EmberScreen* screen, const char* filePath)
	{
		if (filePath == NULL)
		{
			return NULL;
		}
		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(filePath);
		if (Surface == NULL) 
		{
			std::cout << "Could Not Load Texture: " << filePath << std::endl;
			screen->Close();
		}
		else
		{
			std::cout << "Loaded Texture: " << filePath << std::endl;
		}

		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(screen->properties()->m_Renderer, Surface);
		SDL_FreeSurface(Surface);

		return mainTexture;
	}

	SDL_Texture* create_texture_with_surface(EmberScreen* screen, SDL_Surface* surface)
	{
		SDL_Texture* mainTexture = SDL_CreateTextureFromSurface(screen->properties()->m_Renderer, surface);
		SDL_FreeSurface(surface);

		std::cout << "Created Texture From Surface" << std::endl;

		return mainTexture;
	}

	SDL_Surface* load_surface(const char* filePath)
	{
		SDL_Surface* Surface = NULL;
		Surface = IMG_Load(filePath);
		if (Surface == NULL) 
		{
			std::cout << "Could Not Load Surface: " << filePath << std::endl;
		}
		else
		{
			std::cout << "Loaded Surface: " << filePath << std::endl;
		}

		return Surface;
	}

	void draw_no_source(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, float angle, const SDL_RendererFlip& flip)
	{
		SDL_RenderCopyExF(screen->properties()->m_Renderer, texture, NULL, &dest, angle, NULL, flip);
	}

	void draw_with_source(EmberScreen* screen, SDL_Texture* texture, const SDL_FRect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle)
	{
		SDL_RenderCopyExF(screen->properties()->m_Renderer, texture, &src, &dest, angle, NULL, flip);
	}

	void draw_no_source(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, float angle, const SDL_RendererFlip& flip)
	{
		SDL_RenderCopyEx(screen->properties()->m_Renderer, texture, NULL, &dest, angle, NULL, flip);
	}

	void draw_with_source(EmberScreen* screen, SDL_Texture* texture, const SDL_Rect& dest, const SDL_Rect& src, const SDL_RendererFlip& flip, float angle)
	{
		SDL_RenderCopyEx(screen->properties()->m_Renderer, texture, &src, &dest, angle, NULL, flip);
	}

	void set_window_icon(EmberScreen* screen, const char* filePath)
	{
		SDL_Surface* icon = NULL;
		icon = IMG_Load(filePath);
		if (icon == NULL)
		{
			std::cout << "Could Not Load Window Icon Surface:  " << filePath << std::endl;
			screen->Close();
		}
		else
		{
			std::cout << "Loaded Window Icon Surfcace: " << filePath << std::endl;
		}
		SDL_SetWindowIcon(screen->properties()->m_Window, icon);
		SDL_FreeSurface(icon);
	}

	void delete_texture(SDL_Texture* texture)
	{
		SDL_DestroyTexture(texture);
	}

	void set_texture_color(SDL_Texture* texture, int r, int g, int b)
	{
		SDL_SetTextureColorMod(texture, r, g, b);
	}

	void set_alptha(SDL_Texture* texture, int a)
	{
		SDL_SetTextureAlphaMod(texture, a);
	}

	AudioChunk::AudioChunk(const char* filePath)
		: m_ChunkName(Mix_LoadWAV(filePath)), m_CurrentVolume(0)
	{
		if (m_ChunkName == NULL)
			std::cout << "Could Not Load Chunk: " << filePath << std::endl;
	}
	void AudioChunk::play_chunk()
	{
		Mix_PlayChannel(-1, m_ChunkName, 0);
	}

	void AudioChunk::set_volume(unsigned int volume)
	{
		m_CurrentVolume = (volume < 128) ? volume : 128;
		Mix_VolumeChunk(m_ChunkName, m_CurrentVolume);
	}

	AudioChunk::~AudioChunk()
	{
		Mix_FreeChunk(m_ChunkName);
	}

	AudioMusic::AudioMusic(const char* filePath)
		: m_Music(Mix_LoadMUS(filePath)), m_CurrentVolume(0)
	{
		if (m_Music == NULL)
			std::cout << "Could Not Load Music: " << filePath << std::endl;
	}

	void AudioMusic::play_music()
	{
		Mix_PlayMusic(m_Music, -1);
	}

	void AudioMusic::set_volume(unsigned int volume)
	{
		m_CurrentVolume = (volume < 128) ? volume : 128;
		Mix_VolumeMusic(m_CurrentVolume);
	}

	AudioMusic::~AudioMusic()
	{
		Mix_FreeMusic(m_Music);
	}

	void pixel(SDL_Renderer* renderer, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawPoint(renderer, x, y);	
	}

	void horizontal_line(SDL_Renderer* renderer, int x1, int x2, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y1);		
	}

	void vertical_line(SDL_Renderer* renderer, int y1, int y2, int x1, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x1, y2);	
	}

	void line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);		
	}

	void rectangle_border(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_Rect Position = { x, y, w, h };
		SDL_RenderDrawRect(renderer, &Position);
		
	}

	void rectangle_fill(SDL_Renderer* renderer, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_Rect Position = { x, y, w, h };
		SDL_RenderFillRect(renderer, &Position);
		
	}

	void rectangle_borderF(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_FRect Position = { x, y, w, h };
		SDL_RenderDrawRectF(renderer, &Position);

	}

	void rectangle_fillF(SDL_Renderer* renderer, float x, float y, float w, float h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_FRect Position = { x, y, w, h };
		SDL_RenderFillRectF(renderer, &Position);

	}

	void fill_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
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

	void draw_circle(SDL_Renderer* renderer, int centreX, int centreY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
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

	void arcs(SDL_Renderer* renderer, int x0, int y0, int radiusX, int radiusY, int quad1, int quad2, int quad3, int quad4, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		float pih = 3.14 / 2.0f;

		const int prec = 27;
		float theta = 0;

		float x = (float)radiusX * cos(theta);
		float y = (float)radiusY * sin(theta);
		int x1 = (int)x;
		int y1 = (int)y;

		float step = pih / (float)prec;

		for (theta = step; theta <= pih; theta += step) {
			x1 = (float)radiusX * cosf(theta) + 0.5;
			y1 = (float)radiusY * sinf(theta) + 0.5;
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, r, g, b, a);
			if (((int)x != x1) || ((int)y != y1)) {
				if (quad1)
					SDL_RenderDrawLine(renderer, x0 + (int)x, y0 - (int)y, x0 + x1, y0 - y1);
				if (quad2)
					SDL_RenderDrawLine(renderer, x0 - (int)x, y0 - (int)y, x0 - x1, y0 - y1);
				if (quad3)
					SDL_RenderDrawLine(renderer, x0 - (int)x, y0 + (int)y, x0 - x1, y0 + y1);
				if (quad4)
					SDL_RenderDrawLine(renderer, x0 + (int)x, y0 + (int)y, x0 + x1, y0 + y1);
			}

			x = (float)x1;
			y = (float)y1;
		}		
	}

	void rounded_rectangle_border(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		arcs(renderer, x1 + radius, y1 + radius, radius, radius, 0, 1, 0, 0, r, g, b, a);
		horizontal_line(renderer, x1 + radius, x2 - radius, y1, r, g, b, a);
		arcs(renderer, x2 - radius, y1 + radius, radius, radius, 1, 0, 0, 0, r, g, b, a);
		vertical_line(renderer, y1 + radius, y2 - radius, x1, r, g, b, a);
		arcs(renderer, x1 + radius, y2 - radius, radius, radius, 0, 0, 1, 0, r, g, b, a);
		vertical_line(renderer, y1 + radius, y2 - radius, x2, r, g, b, a);
		arcs(renderer, x2 - radius, y2 - radius, radius, radius, 0, 0, 0, 1, r, g, b, a);
		horizontal_line(renderer, x1 + radius, x2 - radius, y2, r, g, b, a);
		
	}

	void rounded_rectangle_fill(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		y1 += radius;
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		rounded_rectangle_border(renderer, x1, y1, x2, y2, radius, r, g, b);
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y1 + (int)dy, x1 + (int)dx / 4 + radius, y1 + (int)dy);
		}
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y1 + (int)dy, x2 + (int)dx - radius, y1 + (int)dy);
		}
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y2 - (int)dy, x1 + (int)dx / 4 + radius, y2 - (int)dy);
		}
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y2 - (int)dy, x2 + (int)dx - radius, y2 - (int)dy);
		}
		SDL_Rect MainBody = { x1, y1 + radius, x2 - x1, y2 - y1 - radius * 2 };
		SDL_Rect TopBody = { x1 + radius, y1, x2 - x1 - radius * 2, radius };
		SDL_Rect BottomBody = { x1 + radius, y2 - radius, x2 - x1 - radius * 2, radius };

		SDL_RenderFillRect(renderer, &MainBody);
		SDL_RenderFillRect(renderer, &TopBody);
		SDL_RenderFillRect(renderer, &BottomBody);
	}

	void rounded_rectangle_border_gui(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 rh, Uint8 gh, Uint8 bh)
	{
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		arcs(renderer, x1 + radius, y1 + radius, radius, radius, 0, 1, 0, 0, rh, gh, bh, 255);
		horizontal_line(renderer, x1 + radius, x2 - radius, y1, rh, gh, bh, 255);
		arcs(renderer, x2 - radius, y1 + radius, radius, radius, 1, 0, 0, 0, rh, gh, bh, 255);
		vertical_line(renderer, y1 + radius, y2 - radius, x1, r, g, b, 255);
		arcs(renderer, x1 + radius, y2 - radius, radius, radius, 0, 0, 1, 0, r, g, b, 255);
		vertical_line(renderer, y1 + radius, y2 - radius, x2, r, g, b, 255);
		arcs(renderer, x2 - radius, y2 - radius, radius, radius, 0, 0, 0, 1, r, g, b, 255);
		horizontal_line(renderer, x1 + radius, x2 - radius, y2, r, g, b, 255);
	}

	void rounded_rectangle_fill_gui(SDL_Renderer* renderer, int headerSize, int x1, int y1, int x2, int y2, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 rh, Uint8 gh, Uint8 bh)
	{
		y1 += radius;
		EmberAssert(renderer != NULL, "Shape function can not take in a NULL renderer");

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		rounded_rectangle_border_gui(renderer, x1, y1 - radius, x2, y2, radius, r, g, b, rh, gh, bh);

		SDL_SetRenderDrawColor(renderer, rh, gh, bh, 255);
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y1 + (int)dy - radius, x1 + (int)dx / 4 + radius, y1 + (int)dy - radius);
		}
		SDL_SetRenderDrawColor(renderer, rh, gh, bh, 255);
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y1 + (int)dy - radius, x2 + (int)dx - radius, y1 + (int)dy - radius);
		}
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x1 - (int)dx + radius, y2 - (int)dy, x1 + (int)dx / 4 + radius, y2 - (int)dy);
		}
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		for (double dy = 1; dy <= radius; dy += 1.0) {
			double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
			SDL_RenderDrawLine(renderer, x2 - (int)dx / 4 - radius, y2 - (int)dy, x2 + (int)dx - radius, y2 - (int)dy);
		}

		SDL_Rect MainBody = { x1, y1 + radius, x2 - x1, y2 - y1 - radius * 2 };

		SDL_Rect BottomBody = { x1 + radius, y2 - radius, x2 - x1 - radius * 2, radius };

		SDL_Rect Header;
		SDL_Rect TopBody;

		if (radius == 0)
		{
			TopBody = { 0, 0, 0, 0 };
			Header = { x1, y1, x2 - x1 + 1, headerSize };
		}
		else
		{
			TopBody = { x1 + radius, y1 - radius, x2 - x1 - radius * 2, radius };
			Header = { x1, y1, x2 - x1 + 1, headerSize - radius };
			MainBody = { x1, y1, x2 - x1, y2 - y1 - radius };
		}

		SDL_RenderFillRect(renderer, &MainBody);
		SDL_SetRenderDrawColor(renderer, rh, gh, bh, 255);
		SDL_RenderFillRect(renderer, &TopBody);
		SDL_RenderFillRect(renderer, &Header);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderFillRect(renderer, &BottomBody);
	}

	Button::Button(EmberScreen* screen, SDL_Rect button)
		:m_CurrentScreen(screen), m_Position(button), m_InOut(1), m_Clicked(false)
	{
	}

	Button::Button(EmberScreen* screen, int x, int y, int w, int h)
		:m_CurrentScreen(screen), m_Position({ x, y, w, h }), m_InOut(1), m_Clicked(false)
	{
	}

	bool Button::hover()
	{
		if (m_CurrentScreen->events()->mouse.x >= m_Position.x && m_CurrentScreen->events()->mouse.x <= m_Position.x + m_Position.w && m_CurrentScreen->events()->mouse.y >= m_Position.y
			&& m_CurrentScreen->events()->mouse.y <= m_Position.y + m_Position.h) {
			return true;
		}
		else
			return false;
		return true;
	}

	bool Button::click(int ID)
	{
		if (m_CurrentScreen->events()->mouse.down && m_CurrentScreen->events()->mouse.id == ID) {
			if (m_CurrentScreen->events()->mouse.x >= m_Position.x && m_CurrentScreen->events()->mouse.x <= m_Position.x + m_Position.w && m_CurrentScreen->events()->mouse.y >= m_Position.y
				&& m_CurrentScreen->events()->mouse.y <= m_Position.y + m_Position.h) {
				m_CurrentScreen->events()->mouse.down = false;
				return true;
			}
		}
		return false;
	}

	int Button::in_out_click(int ID)
	{
		if (m_CurrentScreen->events()->mouse.down && m_CurrentScreen->events()->mouse.id == ID) {
			if (m_Clicked == false && m_CurrentScreen->events()->mouse.x >= m_Position.x && m_CurrentScreen->events()->mouse.x <= m_Position.x + m_Position.w
				&& m_CurrentScreen->events()->mouse.y >= m_Position.y && m_CurrentScreen->events()->mouse.y <= m_Position.y + m_Position.h) {
				m_Clicked = true;
				m_CurrentScreen->events()->mouse.down = false;
				m_InOut = 2;
			}
			else if (m_Clicked == true && m_CurrentScreen->events()->mouse.x >= m_Position.x && m_CurrentScreen->events()->mouse.x <= m_Position.x + m_Position.w
				&& m_CurrentScreen->events()->mouse.y >= m_Position.y && m_CurrentScreen->events()->mouse.y <= m_Position.y + m_Position.h) {
				m_Clicked = false;
				m_CurrentScreen->events()->mouse.down = false;
				m_InOut = 1;
			}
		}
		return m_InOut;
	}

	void Button::update_position(int x, int y)
	{
		m_Position.x = x;
		m_Position.y = y;
	}

	void Button::update_size(int w, int h)
	{
		m_Position.w = w;
		m_Position.h = h;
	}

	bool Button::hold(int ID)
	{
		if (m_CurrentScreen->events()->mouse.down && m_CurrentScreen->events()->mouse.id == ID) {
			if (m_CurrentScreen->events()->mouse.x >= m_Position.x && m_CurrentScreen->events()->mouse.x <= m_Position.x + m_Position.w
				&& m_CurrentScreen->events()->mouse.y >= m_Position.y && m_CurrentScreen->events()->mouse.y <= m_Position.y + m_Position.h) {
				return true;
			}
		}
		return false;
	}

	Grid::Grid(EmberScreen* screen, int x, int y, unsigned int row, unsigned int col, int widthSize, int heightSize)
		:m_CurrentScreen(screen), m_Rows(row), m_Cols(col), m_BlockWidth(widthSize), m_BlockHeight(heightSize),
		m_X(x), m_Y(y), m_StartX(x), m_StartY(y), m_GridButtons(screen, x, y, m_BlockWidth, m_BlockHeight)
	{
	}

	void Grid::resize_define(int x, int y, int widthSize, int heightSize)
	{
		m_X = x;
		m_Y = y;

		m_BlockWidth = widthSize;
		m_BlockHeight = heightSize;
	}

	void Grid::render_rects(Uint8 r, Uint8 g, Uint8 b)
	{
		m_X = m_StartX;
		m_Y = m_StartY;
		int l = -1;
		for (unsigned int i = 0; i < m_Rows; i++) 
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
				l++;

				rectangle_fill(m_CurrentScreen->properties()->m_Renderer, m_X + 1, m_Y + 1, m_BlockWidth - 1, m_BlockHeight - 1, r, g, b);

				m_X += m_BlockWidth;
				if (l == m_Rows - 1) {
					m_Y += m_BlockHeight;
					m_X = m_StartX;
					l = -1;
				}
			}
		}
	}

	void Grid::render_border(Uint8 r, Uint8 g, Uint8 b)
	{
		m_X = m_StartX;
		m_Y = m_StartY;

		for (unsigned int i = 0; i < m_Rows; i++)
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
				horizontal_line(m_CurrentScreen->properties()->m_Renderer, m_X, m_X + m_BlockWidth, m_Y, r, g, b);
				vertical_line(m_CurrentScreen->properties()->m_Renderer, m_Y, m_Y + m_BlockHeight, m_X, r, g, b);

				m_X += m_BlockWidth;
			}
			m_Y += m_BlockHeight;
			m_X = m_StartX;
		}
		horizontal_line(m_CurrentScreen->properties()->m_Renderer, m_StartX, m_StartX + (m_BlockWidth * m_Cols), m_StartY + (m_BlockHeight * m_Rows), r, g, b);
		vertical_line(m_CurrentScreen->properties()->m_Renderer, m_StartY, m_StartY + (m_BlockHeight * m_Rows), m_StartX + (m_BlockWidth * m_Cols), r, g, b);
	}

	std::pair<int, int> Grid::hover()
	{
		m_X = m_StartX;
		m_Y = m_StartY;

		for (unsigned int i = 0; i < m_Rows; i++)
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
				m_GridButtons.update_position(m_X, m_Y);
				if (m_GridButtons.hover())
				{
					return std::make_pair(i, j);
				}

				m_X += m_BlockWidth;
			}
			m_Y += m_BlockHeight;
			m_X = m_StartX;
		}
		return std::make_pair(-1, -1);
	}

	std::pair<int, int> Grid::click(int ID)
	{
		m_X = m_StartX;
		m_Y = m_StartY;

		for (unsigned int i = 0; i < m_Rows; i++)
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
				m_GridButtons.update_position(m_X, m_Y);
				if (m_GridButtons.click(ID))
				{
					return std::make_pair(i, j);
				}
				m_X += m_BlockWidth;
			}
			m_Y += m_BlockHeight;
			m_X = m_StartX;
		}
		return std::make_pair(-1, -1);
	}

	SpriteSheet::SpriteSheet(EmberScreen* screen, char const* filePath, int row, int column)
	{
		m_CurrentScreen = screen;
		m_Sheet = load_texture(m_CurrentScreen, filePath);

		SDL_QueryTexture(m_Sheet, NULL, NULL, &m_Clip.w, &m_Clip.h);

		m_Clip.w /= column;
		m_Clip.h /= row;

		m_Row = row;
		m_Col = column;
		
	}

	SpriteSheet::~SpriteSheet()
	{
		delete_texture(m_Sheet);
	}

	void SpriteSheet::select_sprite(int x, int y)
	{
		m_Clip.x = x * m_Clip.w;
		m_Clip.y = y * m_Clip.h;
	}

	void SpriteSheet::draw_selected_sprite(SDL_Rect position)
	{
		SDL_RenderCopy(m_CurrentScreen->properties()->m_Renderer, m_Sheet, &m_Clip, &position);
	}

	TileMap::TileMap(EmberScreen* screen, int x, int y, unsigned int row, unsigned int col, int widthSize, int heightSize)
		:Grid(screen, x, y, row, col, widthSize, heightSize), m_Counter(0), m_CurrentTexture(-1), m_Pos({x, y, widthSize, heightSize}), m_File("res/Tilemap.txt")
	{
		m_EachTileTexture = new uint32_t * [row];
		m_EachTileSheet = new uint32_t * [row];
		for (size_t i = 0; i < row; i++)
		{
			m_EachTileTexture[i] = new uint32_t[col];
			m_EachTileSheet[i] = new uint32_t[col];
		}

		if (m_File.is_empty())
		{
			for (size_t i = 0; i < row; i++)
			{
				for (size_t j = 0; j < col; j++)
				{
					m_EachTileTexture[i][j] = -1;

					m_File.write_at_end_of_file(-1, true);
					std::stringstream ss(m_File.read_word((row * i) + j));
					ss >> m_EachTileSheet[i][j];
				}
			}
		}
		else	
		{
			for (size_t i = 0; i < row; i++)
			{
				for (size_t j = 0; j < col; j++)
				{
					m_EachTileTexture[i][j] = -1;

					std::stringstream ss(m_File.read_word((row * i) + j));
					ss >> m_EachTileSheet[i][j];
				}
			}
		}
	}

	void TileMap::RenderTexturePack()
	{
		SDL_Rect pos = { m_StartX, m_StartY, m_BlockWidth, m_BlockHeight };
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			m_GridButtons.update_position(pos.x, pos.y);
			draw_no_source(m_CurrentScreen, m_Textures[i].texture, pos);

			if (m_GridButtons.hover())
			{
				rectangle_fill(m_CurrentScreen->properties()->m_Renderer, pos.x + 1, pos.y + 1, pos.w - 1, pos.h - 1, 97, 200, 255, 100);
			}
			if (m_GridButtons.click(LeftMouseButton))
			{
				m_CurrentTexture = m_Textures[i].id;
			}
			if (pos.x / m_BlockWidth < (int)m_Cols - 1)
			{
				pos.x += m_BlockWidth;
			}
			else
			{
				pos.x = m_StartX;
				pos.y += m_BlockHeight;
			}
		}

		m_LastPosition = { (float)pos.x, (float)pos.y };
	}

	void TileMap::AddTexture(const char* filePath)
	{
		m_Textures.push_back({ load_texture(m_CurrentScreen, filePath), m_Counter });
		m_Counter++;
	}

	void TileMap::EditGrid()
	{
		m_Pos.x = m_StartX;
		m_Pos.y = m_StartY;

		std::pair<int, int> Loc;
		Loc = click(LeftMouseButton);

		if (Loc != std::make_pair(-1, -1) && m_EachTileTexture[Loc.first][Loc.second] != m_CurrentTexture)
		{
			m_EachTileTexture[Loc.first][Loc.second] = m_CurrentTexture;
		}

		Loc = click(RightMouseButton);
		if (Loc != std::make_pair(-1, -1))
		{
			m_EachTileTexture[Loc.first][Loc.second] = -1;
		}

		for (unsigned int i = 0; i < m_Rows; i++)
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
					
				if (m_CurrentTexture != -1 && m_EachTileTexture[i][j] != -1)
				{
					draw_no_source(m_CurrentScreen, m_Textures[m_EachTileTexture[i][j]].texture, m_Pos);
				}
				
				m_Pos.x += m_BlockWidth;
			}
			m_Pos.y += m_BlockHeight;
			m_Pos.x = m_StartX;
		}
	}

	void TileMap::RenderSheet(SpriteSheet& spriteSheet, int x, int y)
	{
		SDL_Rect pos = { x, y, m_BlockWidth, m_BlockHeight};

		for (size_t i = 0; i < (size_t)spriteSheet.Row(); i++)
		{
			for (size_t j = 0; j < (size_t)spriteSheet.Col(); j++)
			{
				spriteSheet.select_sprite(i, j);
				spriteSheet.draw_selected_sprite(pos);
				m_GridButtons.update_position(pos.x, pos.y);

				if(m_GridButtons.hover())
				{
					rectangle_fill(m_CurrentScreen->properties()->m_Renderer, pos.x + 1, pos.y + 1, pos.w - 1, pos.h - 1, 97, 200, 255, 100);
				}

				if (m_GridButtons.click(LeftMouseButton))
				{
					m_CurrentSheet = { (float)i, (float)j };
				}
			
				pos.x += m_BlockWidth;			
			}
			pos.y += m_BlockHeight;
			pos.x = x;
		}

		rectangle_fill(m_CurrentScreen->properties()->m_Renderer, x + ((int)m_CurrentSheet.y * m_BlockWidth) + 1, y + ((int)m_CurrentSheet.x * m_BlockHeight) + 1, pos.w - 1, pos.h - 1, 97, 200, 255, 100);
	}

	void TileMap::UseSheet(SpriteSheet& spriteSheet)
	{
		m_Pos.x = m_StartX;
		m_Pos.y = m_StartY;

		std::pair<int, int> Loc;
		Loc = click(LeftMouseButton);

		if (Loc != std::make_pair(-1, -1))
		{
			m_EachTileSheet[Loc.first][Loc.second] = (uint32_t)(spriteSheet.Col() * m_CurrentSheet.y) + (uint32_t)m_CurrentSheet.x;
		}

		Loc = click(RightMouseButton);
		if (Loc != std::make_pair(-1, -1))
		{
			m_EachTileSheet[Loc.first][Loc.second] = -1;
		}

		for (unsigned int i = 0; i < m_Rows; i++)
		{
			for (unsigned int j = 0; j < m_Cols; j++)
			{
				if (m_EachTileSheet[i][j] != -1)
				{
					spriteSheet.select_sprite(m_EachTileSheet[i][j] % spriteSheet.Col(), m_EachTileSheet[i][j] / spriteSheet.Row());
					spriteSheet.draw_selected_sprite({ m_Pos.x, m_Pos.y, m_BlockWidth, m_BlockHeight });
				}

				m_GridButtons.update_position(m_Pos.x, m_Pos.y);
				if (m_GridButtons.hover())
				{
					rectangle_fill(m_CurrentScreen->properties()->m_Renderer, m_Pos.x, m_Pos.y, m_BlockWidth, m_BlockHeight, 97, 200, 255, 100);
				}

				m_Pos.x += m_BlockWidth;
			}
			m_Pos.y += m_BlockHeight;
			m_Pos.x = m_StartX;
		}
	}

	TileMap::~TileMap()
	{
		for (auto& it : m_Textures)
		{
			delete_texture(it.texture);
		}

		m_File.empty_file();
		for (size_t i = 0; i < m_Rows; i++)
		{     
			for (size_t j = 0; j < m_Cols; j++)
			{
				m_File.write_at_end_of_file(m_EachTileSheet[i][j], true);
			}
		} 

		for (size_t i = 0; i < m_Rows; i++)
		{
			delete[] m_EachTileTexture[i];
			delete[] m_EachTileSheet[i];
		}
		delete[] m_EachTileTexture;
		delete[] m_EachTileSheet;

	}

	EmberVec2::EmberVec2(const float x, const float y)
		:x(x), y(y)
	{
	}

	std::ostream& operator<<(std::ostream& os, const EmberVec2& vec2)
	{
		os << vec2.x << " " << vec2.y;
		return os;
	}

	EmberVec2 operator+(const EmberVec2& v1, const EmberVec2& v2)
	{
		return (EmberVec2(v1.x + v2.x, v1.y + v2.y));
	}

	EmberVec2 operator-(const EmberVec2& v1, const EmberVec2& v2)
	{
		return (EmberVec2(v1.x - v2.x, v1.y - v2.y));
	}

	EmberVec2 operator*(const EmberVec2& v1, const EmberVec2& v2)
	{
		return (EmberVec2(v1.x * v2.x, v1.y * v2.y));
	}

	EmberVec2 operator/(const EmberVec2& v1, const EmberVec2& v2)
	{
		return (EmberVec2(v1.x / v2.x, v1.y / v2.y));
	}

	EmberVec2 operator+(const EmberVec2& v1, const float s)
	{
		return (EmberVec2(v1.x + s, v1.y + s));
	}

	EmberVec2 operator-(const EmberVec2& v1, const float s)
	{
		return (EmberVec2(v1.x - s, v1.y - s));
	}

	EmberVec2 operator*(const EmberVec2& v1, const float s)
	{
		return (EmberVec2(v1.x * s, v1.y * s));
	}

	EmberVec2 operator/(const EmberVec2& v1, const float s)
	{
		return (EmberVec2(v1.x / s, v1.y / s));
	}

	void EmberVec2::operator+=(const float scalar)
	{
		this->x += scalar;
		this->y += scalar;
	}

	void EmberVec2::operator-=(const float scalar)
	{
		this->x -= scalar;
		this->y -= scalar;
	}

	void EmberVec2::operator*=(const float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
	}

	void EmberVec2::operator/=(const float scalar)
	{
		this->x /= scalar;
		this->y /= scalar;
	}

	void EmberVec2::operator+=(const EmberVec2& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
	}

	void EmberVec2::operator-=(const EmberVec2& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void EmberVec2::operator*=(const EmberVec2& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
	}

	void EmberVec2::operator/=(const EmberVec2& vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
	}

	void EmberVec2::negate()
	{
		this->x *= -1;
		this->y *= -1;
	}

	float EmberVec2::magnitude()
	{
		return (std::sqrt((this->x * this->x) + (this->y * this->y)));
	}

	void EmberVec2::normalize()
	{
		float m = magnitude();
		this->x /= m;
		this->y /= m;
	}

	float EmberVec2::dot_product(EmberVec2& vec2)
	{
		return ((this->x * vec2.x) + (this->y * vec2.y));
	}

	std::vector<File*> m_AllFiles;   
	
	bool BindFileToManager(File* file)
	{
		m_AllFiles.push_back(file);             
		return true;
	}

	void CheckForDoubles(const char* fileClosingFrom)
	{
		for (auto& it : m_AllFiles)
		{
			if (it->get_path() == fileClosingFrom)
			{
				it->close_file();
			}
		}
	}

	void CloseAllFiles()
	{
		for (auto it : m_AllFiles)
		{
			it->close_file();
		}
	}

	File::File(const char* filePath)
		: m_FilePath(filePath), m_CurrentSize(0), m_FillInMemoryData(true)
	{
		CheckForDoubles(m_FilePath);
		if (BindFileToManager(this))
		{
			open();
			get_size_in_bytes();
		}
	}

	File::~File()
	{
		close_file();
	}

	void File::open()
	{
		m_FileData = std::fstream(m_FilePath, std::fstream::in | std::fstream::out | std::fstream::app);
		EmberAssert(m_FileData.is_open(), "Could not open file");
	}

	void File::close_file()
	{
		m_FileData.close();
	}

	void File::delete_file()
	{
		m_FileData.close();
		remove(m_FilePath);
	}

	void File::empty_file()
	{
		m_FileData.close();
		m_FileData.open(m_FilePath, std::fstream::out | std::fstream::trunc);
		close_file();
		open();
		EmberAssert(m_FileData.is_open(), "Could not open file");
	}

	std::streamoff File::get_size_in_bytes()
	{
		EmberAssert(m_FileData.is_open(), "Could not open file");

		std::streampos beginningOfFile, endOfFile;
		m_FileData.seekg(0, std::ios::beg);
		beginningOfFile = m_FileData.tellg();
		m_FileData.seekg(0, std::ios::end);
		endOfFile = m_FileData.tellg();

		m_CurrentSize = endOfFile - beginningOfFile;

		return endOfFile - beginningOfFile;
	}

	std::streampos File::get_current_location()
	{
		return m_FileData.tellg();
	}

	std::string File::read_all_file()
	{
		before_reading();

		m_FileData.seekg(0, std::ios::end);
		std::streampos sizeOfWholeFile = get_current_location();
		std::string bufferOfWholeFile((size_t)sizeOfWholeFile, ' ');
		m_FileData.seekg(0);
		m_FileData.read(&bufferOfWholeFile[0], sizeOfWholeFile);

		m_Data = bufferOfWholeFile;

		m_FillInMemoryData = false;
		return bufferOfWholeFile;
	}


	bool File::check_if_file_empty()
	{
		return (get_size_in_bytes() == 0) ? true : false;
	}

	void File::change_file_location_pointer(int location)
	{
		EmberAssert(m_FileData.is_open(), "Could not open file");
		m_FileData.seekg(location, std::ios::beg);
	}

	void File::reset_location()
	{
		EmberAssert(m_FileData.is_open(), "Could not open file");
		m_FileData.clear();
		m_FileData.seekg(std::ios::beg);
	}

	std::string File::read_line(int lineNumber)
	{
		before_reading();

		std::string searchLineOfFile;
		unsigned int lineCounterForFile = 0;

		while (getline(m_FileData, searchLineOfFile))
		{
			if (lineCounterForFile == lineNumber)
			{
				return searchLineOfFile;
			}
			lineCounterForFile++;
		}

		return m_FilePath;
	}

	std::string File::get_word_from_location(int location)
	{
		before_reading();

		unsigned int wordCounterForFile = 0;
		std::string searchWordOfFile;

		while (m_FileData >> searchWordOfFile)
		{
			if (wordCounterForFile == location)
			{
				return searchWordOfFile;
			}
			wordCounterForFile++;
		}

		return m_FilePath;
	}

	unsigned int File::get_word_count()
	{
		before_reading();

		std::string searchWordOfFile;
		unsigned int wordCounterForFile = 0;
		while (m_FileData >> searchWordOfFile)
		{
			wordCounterForFile++;
		}

		return wordCounterForFile;
	}

	__int64 File::get_line_count()
	{
		before_reading();

		return std::count(std::istreambuf_iterator<char>(m_FileData),
			std::istreambuf_iterator<char>(), '\n') + 1;
	}

	std::string File::read_word(int location)
	{
		before_reading();

		std::string searchWordOfFile;
		unsigned int wordCounterForFile = 0;
		while (m_FileData >> searchWordOfFile)
		{
			if (wordCounterForFile == location)
			{
				return searchWordOfFile;
			}
			wordCounterForFile++;
		}

		return m_FilePath;
	}

	unsigned int File::get_location_from_word(const char* word)
	{
		before_reading();

		std::string searchWordOfFile;
		unsigned int wordCounterForFile = 0;
		while (m_FileData >> searchWordOfFile)
		{
			if (searchWordOfFile == word)
			{
				return wordCounterForFile;
			}
			wordCounterForFile++;
		}

		return -1;
	}

	void File::before_reading()
	{
		EmberAssert(m_FileData.is_open(), "Could not open file");
		reset_location();
	}

	void File::before_writing()
	{
		EmberAssert(m_FileData.is_open(), "Could not open file");
		if (m_FillInMemoryData)
		{
			read_all_file();
			m_FillInMemoryData = false;
		}
		reset_location();
	}

	template <typename _Type>
	void File::write_at_end_of_file(const _Type& data, bool newLine)
	{
		before_writing();
		std::stringstream buffer;
		buffer << data;

		if (newLine)
		{
			buffer << '\n';
		}

		m_FileData << buffer.str();
		get_size_in_bytes();
		m_FillInMemoryData = true;
	}

	unsigned long File::get_character_count()   //includes '\n' and ' ' for now (FIX)
	{
		before_reading();

		char activeCharacterBeingRead;
		unsigned long numberofCharacters = 0;

		while (m_FileData.get(activeCharacterBeingRead))
		{
			numberofCharacters++;
		}

		return numberofCharacters;
	}

	template <typename _Type>
	void File::write_at_end_of_word(const _Type& data, const char* finderWord)
	{
		before_writing();

		size_t locationOfFoundWord = m_Data.find(finderWord);
		std::stringstream buffer(data);

		if (locationOfFoundWord != std::string::npos)
		{
			m_Data.insert(locationOfFoundWord + strlen(finderWord), buffer.str());
		}

		empty_file();
		m_FileData << m_Data;
		get_size_in_bytes();
		m_FillInMemoryData = true;
	}

	template<typename _Type>
	void File::write_at_end_of_line(const _Type& data, int lineNumber)
	{
		before_writing();

		int counter = 0;
		for (size_t i = 0; i < m_Data.length(); i++)
		{
			if (m_Data[i] == '\n')
			{
				if (lineNumber == counter)
				{
					std::stringstream buffer;
					buffer << data;
					m_Data.insert(i, data);
					break;
				}
				counter++;
			}
		}

		empty_file();
		m_FileData << m_Data;
		get_size_in_bytes();
		m_FillInMemoryData = true;
	}

	bool PointVsRect(const EmberVec2& point, const EmberRect& rect)
	{
		return (point.x >= rect.pos.x && point.y >= rect.pos.y && point.x < rect.pos.x + rect.pos.w && point.y < rect.pos.y + rect.pos.h);
	}

	bool RectVsRect(const EmberRect& rect1, const EmberRect& rect2)
	{
		return (rect1.pos.x < rect2.pos.x + rect2.pos.w && rect1.pos.x + rect1.pos.w > rect2.pos.x &&
			rect1.pos.y < rect2.pos.y + rect2.pos.h && rect1.pos.y + rect1.pos.h > rect2.pos.y);
	}

	bool RayVsRect(const EmberVec2& ray, const EmberVec2& rayDir, const EmberRect& rect, EmberVec2& contact, EmberVec2& contact_normal, float& hitNear)
	{
		EmberVec2 near = (rect.pos.a - ray) / rayDir;
		EmberVec2 far = (rect.pos.a + rect.pos.b - ray) / rayDir;

		if (near.x > far.x)
			std::swap(near.x, far.x);
		if (near.y > far.y)
			std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x) 
			return false;

		hitNear = std::max(near.x, near.y);
		float hitFar = std::min(far.x, far.y);

		if (hitFar < 0)
			return false;

		contact.x = ray.x + hitNear * rayDir.x;
		contact.y = ray.y + hitNear * rayDir.y;

		if (near.x > near.y)
		{
			if (rayDir.x < 0)
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
			if (rayDir.y < 0)
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

	bool DynamicRectVsRect(const EmberRect& rect1, const EmberRect& rect2, EmberVec2& contact, EmberVec2& contact_normal, float& hitNear)
	{
		if (rect1.vel.x == 0 && rect1.vel.y == 0)
			return false;

		EmberRect expand;
		expand.pos.a = rect2.pos.a - rect1.pos.b / 2 + (rect1.pos.b / 2 + 1);
		expand.pos.b = rect2.pos.b + rect1.pos.b - 2;
	
		if (RayVsRect(rect1.pos.a + rect1.pos.b, rect1.vel, expand, contact, contact_normal, hitNear))
		{
			if (hitNear <= 1.0f)
			{
				return true;
			}
		}

		return false;
	}
}
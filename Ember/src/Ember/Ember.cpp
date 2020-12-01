#include "Ember.h"

constexpr int RenderingIndex = -1;

namespace Ember {
	EventStack::EventStack(const std::string& first_name) {
		AddLayer(first_name);
		current_layer = 0;
	}

	EventStack::EventStackError EventStack::SetCurrentLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index != -1) {
			current_layer = index;
			return EventStackError::Null;
		}

		return EventStackError::LayerNameCouldNotBeFound;
	}

	EventStack::EventStackError EventStack::AddLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index == -1) {
			event_layers.push_back(name);
			return EventStackError::Null;
		}
		return EventStackError::DuplicitLayerFound;
	}

	EventStack::EventStackError EventStack::DeleteLayer(const std::string& name) {
		size_t index = IndexLayers(name);
		if (index != -1) {
			event_layers.erase(event_layers.begin() + index);
			return EventStackError::Null;
		}

		return EventStackError::LayerDeletionFailed;
	}

	EventStack::EventStackError EventStack::DrawEventToLayer(const std::function<void()>& event_func, const std::string& name) {
		if (name == event_layers[current_layer].Name()) {
			event_func();
			return EventStackError::Null;
		}
		return EventStackError::LayerNameCouldNotBeFound;
	}

	size_t EventStack::IndexLayers(const std::string& name) {
		for (size_t i = 0; i < event_layers.size(); i++) {
			if (event_layers[i].Name() == name) {
				return i;
			}
		}

		return -1;
	}

#ifdef EMBER_WINDOW_PLATFORM
	wWindow::wWindow(WindowProperties* properties)
		: is_running(false), native_window(nullptr) {
		is_running = Initializer(properties);
		if (is_running) {
			this->properties = properties;
			if (!AssertProperties())
				Destroy();
		}
		else
			Destroy();
	}

	wWindow::~wWindow() {
		Destroy();
	}

	bool wWindow::Initializer(WindowProperties* properties) {
		bool initialize_checker = false;
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			return initialize_checker;
		else
			initialize_checker = Create(properties);

		initialize_checker = InitializeAssets();

		return initialize_checker;
	}

	void wWindow::Destroy() {
		AssetCleanUp();
		SDL_DestroyWindow(native_window);
		SDL_Quit();

		properties = nullptr;
	}

	void wWindow::Update() {
		SDL_SetWindowSize(native_window, properties->width, properties->height);
		SDL_SetWindowTitle(native_window, properties->name.c_str());
		SDL_SetWindowFullscreen(native_window, properties->full_screen);
	}

	void wWindow::SetWindowIcon(const char* file_path) {
		SDL_Surface* icon = Texture::LoadSurface(file_path);
		SDL_SetWindowIcon(native_window, icon);
		SDL_FreeSurface(icon);
	}

	bool wWindow::AssertProperties() {
		return (properties != nullptr);
	}

	bool wWindow::Create(WindowProperties* properties) {
		if (AssertSize(properties->width, properties->height)) {
			CalculatePosition(&properties->position);

			native_window = SDL_CreateWindow(properties->name.c_str(), properties->position.x, properties->position.y, properties->width,
				properties->height, IsFullScreen(properties));
		}

		return (native_window != nullptr);
	}

	Uint32 wWindow::IsFullScreen(WindowProperties* properties) {
		if (properties->full_screen)
			return SDL_WINDOW_FULLSCREEN;
		return 0;
	}

	bool wWindow::AssertSize(uint32_t width, uint32_t height) {
		return (width != 0 && height != 0);
	}

	void wWindow::CalculatePosition(IVec2* position) {
		if (position->x == -1 && position->y == -1) {
			position->x = SDL_WINDOWPOS_CENTERED;
			position->y = SDL_WINDOWPOS_CENTERED;
		}
	}
#endif // EMBER_WINDOWS_PLATFORM

	bool InitializeImageLoader() {
		int imgFlags = IMG_INIT_PNG;

		if (!(IMG_Init(imgFlags) & imgFlags))
			return false;

		return true;
	}

	bool InitializeFontLoader() {
		if (TTF_Init() == -1)
			return false;

		return true;
	}

	bool InitializeSoundLoader() {
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		return true;
	}

	bool InitializeAssets() {
		return (InitializeImageLoader() && InitializeFontLoader() && InitializeSoundLoader());
	}

	void AssetCleanUp() {
		IMG_Quit();
		TTF_Quit();
		Mix_Quit();
	}

	void CheckVersion() {
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);
		std::cout << "We compiled against SDL version: " << compiled.major << compiled.minor << compiled.patch << std::endl;
		std::cout << "But we are linking against SDL version: " << linked.major << linked.minor << linked.patch << std::endl;
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

	SDL_bool ConvertToSDLBool(bool value) {
		if (value)
			return SDL_TRUE;
		else
			return SDL_FALSE;
	}

	rRenderer::rRenderer(Window* window)
		: renderer(nullptr), window(nullptr) {
		if (window != nullptr) {
			this->window = window;
			if (!Initializer())
				Destroy();
		}
	}

	rRenderer::~rRenderer() {
		Destroy();
	}

	SDL_Renderer* rRenderer::Renderer() {
		return renderer;
	}

	bool rRenderer::Initializer() {
		renderer = SDL_CreateRenderer((SDL_Window*)window->GetNativeWindow(), RenderingIndex,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		return (renderer != nullptr);
	}

	void rRenderer::Destroy() {
		SDL_DestroyRenderer(renderer);
	}

	void rRenderer::SetViewport(int x, int y, int w, int h) {
		SDL_Rect view_port = { x, y, w, h };
		SDL_RenderSetViewport(renderer, &view_port);
	}

	void rRenderer::Clear(const Color& color) {
		SetColor(color);
		SDL_RenderClear(renderer);
	}

	void rRenderer::SetColor(const Color& color) {
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}

	void rRenderer::Show() {
		SDL_RenderPresent(renderer);
	}

	void rRenderer::Rectangle(const Rect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderFillRect(renderer, &rect.rect);
	}

	void rRenderer::Border(const Rect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderDrawRect(renderer, &rect.rect);
	}

	void rRenderer::Line(const IVec2& point1, const IVec2& point2, const Color& color) {
		SetColor(color);
		SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
	}

	void rRenderer::Point(const IVec2& point, const Color& color) {
		SetColor(color);
		SDL_RenderDrawPoint(renderer, point.x, point.y);
	}

	void rRenderer::RectangleF(const FRect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderFillRectF(renderer, &rect.rect);
	}

	void rRenderer::BorderF(const FRect& rect, const Color& color) {
		SetColor(color);
		SDL_RenderDrawRectF(renderer, &rect.rect);
	}

	void rRenderer::LineF(const Vec2& point1, const Vec2& point2, const Color& color) {
		SetColor(color);
		SDL_RenderDrawLineF(renderer, point1.x, point1.y, point2.x, point2.y);
	}

	void rRenderer::PointF(const Vec2& point, const Color& color) {
		SetColor(color);
		SDL_RenderDrawPointF(renderer, point.x, point.y);
	}

	Texture::Texture(const char* file_path, rRenderer* renderer)
		: renderer(nullptr), texture(nullptr) {
		if (renderer != nullptr) {
			this->renderer = renderer;
			texture = LoadTexture(renderer->Renderer(), file_path);
		}
	}

	Texture::~Texture() {
		DeleteTexture();
	}

	void Texture::Draw(const FRect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyExF(renderer->Renderer(), texture, NULL, &dest.rect, angle, NULL, flip);
	}

	void Texture::Draw(const FRect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyExF(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, NULL, flip);
	}

	void Texture::Draw(const Rect& dest, float angle, const SDL_RendererFlip& flip) {
		SDL_RenderCopyEx(renderer->Renderer(), texture, NULL, &dest.rect, angle, NULL, flip);
	}

	void Texture::Draw(const Rect& dest, const Rect& src, const SDL_RendererFlip& flip, float angle) {
		SDL_RenderCopyEx(renderer->Renderer(), texture, &src.rect, &dest.rect, angle, NULL, flip);
	}

	SDL_Texture* Texture::LoadTexture(SDL_Renderer* renderer, const char* file_path) {
		if (file_path == nullptr)
			return nullptr;

		SDL_Surface* surface = LoadSurface(file_path);

		return CreateTextureFromSurface(renderer, surface);
	}

	SDL_Texture* Texture::CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface) {
		SDL_Texture* main_texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);

		return main_texture;
	}

	SDL_Surface* Texture::LoadSurface(const char* file_path) {
		SDL_Surface* Surface = nullptr;
		Surface = IMG_Load(file_path);
		if (Surface == nullptr)
			return nullptr;

		return Surface;
	}

	void Texture::DeleteTexture() {
		SDL_DestroyTexture(texture);
	}

	void Texture::TextureColor(const Color& color) {
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}

	void Texture::TextureAlptha(Uint8 a) {
		SDL_SetTextureAlphaMod(texture, a);
	}

	IVec2 Texture::GetTextureInfo() {
		int w, h;
		SDL_QueryTexture(texture, NULL, NULL, &w, &h);
		return { w, h };
	}

	Font::Font(rRenderer* renderer, const char* file_path, const char* text, int size, const Color& color, int x, int y)
		: renderer(renderer), font_width(0), font_height(0), font(TTF_OpenFont(file_path, size)), font_position({ x, y, 0, 0 }),
		font_texture(NULL), font_color(color), font_is_locked(true) {
		if (font) {
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
		if (!font_is_locked) 
			SDL_RenderCopy(renderer->Renderer(), font_texture, NULL, &font_position);
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

	EventHandler::EventHandler(Window* window, Events* events)
		: window(nullptr), events(nullptr), native_event_handler() {
		if (window != nullptr && events != nullptr) {
			this->window = window;
			this->events = events;
		}
	}

	void EventHandler::Update() {
		while (SDL_PollEvent(&native_event_handler)) {
			KeyEvent();
			Quit();
			MouseEvent();
			MousePositonEvent();
			Resize();
		}
	}

	void EventHandler::Quit() {
		if (native_event_handler.type == SDL_QUIT) {
			QuitEvent quit(true);
			callback(quit);
			window->Quit();
		}
	}

	void EventHandler::KeyEvent() {
		if (native_event_handler.type == SDL_KEYDOWN) {
			KeyboardEvents keyboard(true, native_event_handler.key.repeat, native_event_handler.key.keysym.scancode);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
		else if (native_event_handler.type == SDL_KEYUP) {
			KeyboardEvents keyboard(false, 0, 0);
			events->keyboard_event = keyboard;
			callback(keyboard);
		}
	}

	void EventHandler::MouseEvent() {
		if (native_event_handler.type == SDL_MOUSEBUTTONDOWN) {
			MouseButtonEvents mouse(true, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
		else if (native_event_handler.type == SDL_MOUSEBUTTONUP) {
			MouseButtonEvents mouse(false, native_event_handler.button.button, native_event_handler.button.clicks);
			events->mouse_event = mouse;
			callback(mouse);
		}
	}

	void EventHandler::Resize() {
		if (native_event_handler.type == SDL_WINDOWEVENT) {
			if (native_event_handler.window.event == SDL_WINDOWEVENT_RESIZED) {
				ResizeEvent resize(native_event_handler.window.data1, native_event_handler.window.data2);
				window->Properties()->width = resize.w;
				window->Properties()->height = resize.h;
				callback(resize);
			}
		}
		SDL_SetWindowResizable(window->GetNativeWindow(), SDL_TRUE);
	}

	void EventHandler::MousePositonEvent() {
		if (native_event_handler.type == SDL_MOUSEMOTION) {
			MouseButtonPositionEvents pos(native_event_handler.button.x, native_event_handler.button.y);
			events->mouse_pos = pos;
			callback(pos);
		}
	}

	Application::Application(const std::string& name, bool full_screen, uint32_t width, uint32_t height) {
		properties = new WindowProperties(name, width, height);
		properties->full_screen = full_screen;
		window = new wWindow(properties);
		events = new Events();
		event_handler = new EventHandler(window, events);
		event_handler->SetEventCallback(EMBER_BIND_EVENT(OnEvent));

		renderer = new rRenderer(window);
	}

	Application::~Application() {
		delete properties;
		delete window;
		delete events;
		delete event_handler;
		delete renderer;
	}

	void Application::Run() {
		OnCreate();
		while (window->IsRunning()) {
			event_handler->Update();
			OnUserUpdate();
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		dispatcher.Dispatch<QuitEvent>(EMBER_BIND_EVENT(OnClose));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_EVENT(OnResize));
	}

	bool Application::OnClose(const QuitEvent& event) {
		window->Quit();
		return true;
	}

	bool Application::OnResize(const ResizeEvent& event) {
		window->Properties()->width = event.w;
		window->Properties()->height = event.h;
		return true;
	}

	Button::Button(Events* events, const Rect& button)
		: events(events), position(button), in(false), clicked(false) {
	}

	Button::Button(Events* events, int x, int y, int w, int h)
		: events(events), position({ x, y, w, h }), in(false), clicked(false) {
	}

	bool Button::Hover() {
		if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w && events->MousePosition().y >= position.y
			&& events->MousePosition().y <= position.y + position.h) {
			return true;
		}
		return false;
	}

	bool Button::Click(ButtonIds id) {
		if (events->Down() && events->ButtonId() == id && !clicked && Hover()) {
			clicked = true;
			return true;
		}
		if (clicked) {
			if (!events->Down()) 
				clicked = false;
		}
		return false;
	}

	void Button::UpdatePosition(int x, int y) {
		position.x = x;
		position.y = y;
	}

	void Button::UpdateSize(int w, int h) {
		position.w = w;
		position.h = h;
	}

	bool Button::InOut(ButtonIds id) {
		bool click = Click(id);
		if (click && !in) 
			in = true;
		else if (click && in) 
			in = false;

		return in;
	}

	bool Button::Hold(ButtonIds id) {
		if (events->Down() && events->ButtonId() == id) 
			if (events->MousePosition().x >= position.x && events->MousePosition().x <= position.x + position.w
				&& events->MousePosition().y >= position.y && events->MousePosition().y <= position.y + position.h) 
				return true;
		return false;
	}

	Grid::Grid(rRenderer* renderer, Events* events, const GridComponents& grid)
		: renderer(renderer), events(events), grid(grid), button(events, 0, 0, grid.block_width, grid.block_height), x(grid.start_x), y(grid.start_y), camera_position(0, 0) {
	}

	void Grid::ResizeGrid(GridComponents& grid_component) {
		grid.block_width = grid_component.block_width;
		grid.block_height = grid_component.block_height;

		grid.cols = grid_component.cols;
		grid.rows = grid_component.rows;

		grid.start_x = grid_component.start_x;
		grid.start_y = grid_component.start_y;
	}

	void Grid::RenderRect(int col, int row, const Color& color) {
		renderer->Rectangle({ grid.start_x + col * grid.block_width - camera_position.x, grid.start_y + row * grid.block_height - camera_position.y, grid.block_width, grid.block_height }, color);
	}

	void Grid::RenderBorder(const Color& color) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				renderer->Line({ x , y }, { x + grid.block_width, y }, color);
				renderer->Line({ x, y }, { x, y + grid.block_height }, color);

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		renderer->Line({ grid.start_x - camera_position.x, grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, { grid.start_x + (grid.block_width * grid.cols) - camera_position.x,
						 grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, color);
		renderer->Line({ grid.start_x + (grid.block_width * grid.cols) - camera_position.x, grid.start_y - camera_position.y }, { grid.start_x + (grid.block_width * grid.cols) - camera_position.x,
						 grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, color);
	}

	IVec2 Grid::Hover() {  
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hover()) 
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Click(ButtonIds id) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Click(id)) 
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Hold(ButtonIds id) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hold(id)) 
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::ConvertPoints(int col, int row) {
		return { grid.start_x + (col * grid.block_width) - camera_position.x, grid.start_y + (row * grid.block_height) - camera_position.y };
	}

	void Grid::Personalize(const std::function<void(int x, int y, int bw, int bh)>& function) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				function(x, y, grid.block_width, grid.block_height);

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
	}

	SpriteSheet::SpriteSheet(Texture& texture, int rows, int cols)
		: row(rows), col(cols), clip({ 0, 0, 0, 0 }), texture(texture) {
		clip.size = texture.GetTextureInfo();

		clip.w /= col;
		clip.h /= row;
	}

	void SpriteSheet::SelectSprite(int x, int y) {
		clip.x = x * clip.w;
		clip.y = y * clip.h;
	}

	void SpriteSheet::DrawSelectedSprite(const Rect& rect) { 
		texture.Draw(rect, clip);
	}

	Rect SpriteSheet::ReturnSourceRect() {
		return clip;
	}

	TileMapSerializer::TileMapSerializer(GridComponents& grid, const char* file_path)
		: serialier(file_path), index(0), col_count(0), row_count(0), gc_saved(false) {
		PullSize(grid);
		PullGrid(grid);

		Read(grid.start_x);
		Read(grid.start_y);
	}

	void TileMapSerializer::Save(GridComponents& grid, std::vector<TileInfo>& info) {
		if (!gc_saved) {
			serialier.EmptyFile();
			serialier.Write(grid.block_width);
			serialier.Write(" ");
			serialier.Write(grid.block_height);
			serialier.Write(" ");
			serialier.Write(grid.rows);
			serialier.Write(" ");
			serialier.Write(grid.cols);
			serialier.Write(" ");
			serialier.Write(grid.start_x);
			serialier.Write(" ");
			serialier.Write(grid.start_y);
			serialier.Write("\n");
			gc_saved = true;
		}
		for (int i = 0; i < grid.rows; i++) {
			for (int j = 0; j < grid.cols; j++) {
				serialier.Write(info[grid.cols * i + j].texture_id);
				serialier.Write(" ");
			}
			serialier.Write("\n");
		}
	}

	std::vector<TileInfo> TileMapSerializer::AddNewLayer(int col, int row) {
		serialier.Write("\n");
		std::vector<TileInfo> texture_ids;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				serialier.Write(-1);
				serialier.Write(" ");
				texture_ids.push_back({ -1 });
			}
			serialier.Write("\n");
		}

		return texture_ids;
	}

	void TileMapSerializer::ReadTextureInfo(int col, int row, std::vector<TileInfo>& data) {
		unsigned int texture_id_num = col * row;
		serialier.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter < texture_id_num)
				data.push_back({ std::stoi(word) });

			return true;
			});
	}

	size_t TileMapSerializer::ReadLayeredTexture(int col, int row, std::vector<std::vector<TileInfo>>& data) {
		unsigned int texture_id_num = col * row;
		size_t current_layer = 0;
		data.push_back(std::vector<Ember::TileInfo>());
		serialier.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter >= texture_id_num) {
				data.push_back(std::vector<Ember::TileInfo>());
				texture_id_num += texture_id_num;
				current_layer++;
			}
			if (counter < texture_id_num)
				data[current_layer].push_back({ std::stoi(word) });

			return true;
			});

		return current_layer;
	}

	void TileMapSerializer::ClearStream() {
		output.str(std::string());
		output.clear();
	}

	void TileMapSerializer::Read(int& value) {
		ClearStream();

		output << serialier.ReadWord(index);
		output >> value;
		index++;
	}

	void TileMapSerializer::PullSize(GridComponents& grid) {
		Read(grid.block_width);
		Read(grid.block_height);
	}

	void TileMapSerializer::PullGrid(GridComponents& grid) {
		Read(grid.rows);
		row_count = grid.rows;
		Read(grid.cols);
		col_count = grid.cols;
	}

	TileMap::TileMap(rRenderer* renderer, Events* Events, const GridComponents& grid)
		: Grid(renderer, Events, grid) { }

	void TileMap::AddLayer(std::vector<TileInfo>& texture_info) {
		layers_texture_info.push_back(texture_info);
	}

	void TileMap::UploadTextureVector(std::vector<TileInfo>& texture_info, size_t layer_num) {
		if (layer_num < layers_texture_info.size())
			layers_texture_info[layer_num] = texture_info;
	}

	void TileMap::RenderTextures(SpriteSheet& sprite_sheet) {
		for (auto& layer : layers_texture_info) {
			x = grid.start_x - camera_position.x;
			y = grid.start_y - camera_position.y;
			for (int i = 0; i < grid.cols; i++) {
				for (int j = 0; j < grid.rows; j++) {
					if (layer[grid.cols * j + i].texture_id != -1) {
						sprite_sheet.SelectSprite(layer[grid.cols * j + i].texture_id % sprite_sheet.Size().x, layer[grid.cols * j + i].texture_id / sprite_sheet.Size().x);
						sprite_sheet.DrawSelectedSprite({ x, y, grid.block_width, grid.block_height });
					}
					y += grid.block_height;
				}
				x += grid.block_width;
				y = grid.start_y - camera_position.y;
			}
		}
	}

	TileMapEditor::TileMapEditor(TileMap& map)
		: map(map), current_user_texture(-1), texture_picker(map.Eventss(), 0, 0, map.GridComponent().block_width, map.GridComponent().block_height) { }

	void TileMapEditor::UpdateMap(std::vector<TileInfo>& texture_info, ButtonIds btn_id, const IVec2& camera_position) {
		IVec2 click_id = map.Hover();
		map.SetCameraPosition(camera_position);
		if (click_id.x != -1 && click_id.y != -1 && current_user_texture != -1 && map.Eventss()->ButtonId() == btn_id && map.Eventss()->Down()) 
			texture_info[map.GridComponent().cols * click_id.y + click_id.x].texture_id = current_user_texture;
	}

	void TileMapEditor::DeleteTile(std::vector<TileInfo>& texture_info, SDL_Scancode scancode) {
		IVec2 hover_on = map.Hover();
		if (map.Eventss()->KeyCode() == scancode) 
			if(hover_on.x != -1 && hover_on.y != -1)
				texture_info[map.GridComponent().cols * hover_on.y + hover_on.x].texture_id = -1;
	}

	void TileMapEditor::RenderSpriteSheet(SpriteSheet& sprite_sheet, const IVec2& start_location, ButtonIds btn_id, const IVec2& size) {
		for (int i = 0; i < sprite_sheet.Size().x; i++) {
			for (int j = 0; j < sprite_sheet.Size().y; j++) {
				texture_picker.UpdatePosition(start_location.x + i * size.x, start_location.y + j * size.y);
				sprite_sheet.SelectSprite(i, j);
				sprite_sheet.DrawSelectedSprite({ start_location.x + i * size.x, start_location.y + j * size.y,
					size.x, size.y });
				if (texture_picker.Click(btn_id))
					current_user_texture = sprite_sheet.Size().x * j + i;
			}
		}
	}
}
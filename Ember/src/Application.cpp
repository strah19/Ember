#include "Application.h"

namespace Ember {
	void Application::Initialize(const std::string& name, uint32_t width, uint32_t height, AppFlags flags) {
		Ember::LogImpl::Init();
		properties = new WindowProperties(name, width, height);

		properties->full_screen = (flags & AppFlags::FULL_SCREEN) ? true : false;
		window = Window::CreateOpenGLWindow(properties, (flags & OPENGL_CUSTOM_VERSION) ? opengl_major_version : 0, (flags & OPENGL_CUSTOM_VERSION) ? opengl_minor_version : 0);

		event_handler = new EventHandler(window);
		event_handler->SetEventCallback(EMBER_BIND_FUNC(OnEvent));

		OnCreate();
	}

	Application::~Application() {
		delete properties;
		delete window;
		delete event_handler;
	}

	void Application::Run() {
		uint64_t now = SDL_GetPerformanceCounter();
		uint64_t last = 0;
		float delta = 0;

		while (window->IsRunning()) {
			event_handler->Update();

			last = now;
			now = SDL_GetPerformanceCounter();

			delta = (float)((now - last) * 1000 / (float)SDL_GetPerformanceFrequency());
			OnUserUpdate(delta);
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		dispatcher.Dispatch<QuitEvent>(EMBER_BIND_FUNC(OnClose));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(OnResize));
	}

	void Application::OnClose(const QuitEvent& event) {
		window->Quit();
	}

	void Application::OnResize(const ResizeEvent& event) {
		window->Properties()->width = event.w;
		window->Properties()->height = event.h;
	}
}
#include "Application.h"

namespace Ember {
	void Application::Initialize(const std::string& name, uint32_t width, uint32_t height, AppFlags flags) {
		Ember::LogImpl::Init();
		properties = new WindowProperties(name, width, height);

		properties->full_screen = (flags & AppFlags::FULL_SCREEN) ? true : false;
		window = (flags & AppFlags::OPENGL) ? Window::CreateEmberWindow(properties, 4, 5) : Window::CreateEmberWindow(properties);

		event_handler = new EventHandler(window);
		event_handler->SetEventCallback(EMBER_BIND_FUNC(OnEvent));

		renderer = (flags & AppFlags::OPENGL) ?  nullptr : new rRenderer(window);

		OnCreate();
	}

	Application::~Application() {
		delete properties;
		delete window;
		delete event_handler;
		delete renderer;
	}

	void Application::Run() {
		while (window->IsRunning()) {
			event_handler->Update();
			OnUserUpdate();
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
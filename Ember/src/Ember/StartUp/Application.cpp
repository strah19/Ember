#include "Application.h"

namespace Ember {
	void Application::Initialize(const std::string& name, bool full_screen, uint32_t width, uint32_t height) {
		properties = new WindowProperties(name, width, height);
		properties->full_screen = full_screen;
		window = new Window(properties);
		events = new Events();
		event_handler = new EventHandler(window, events);
		event_handler->SetEventCallback(EMBER_BIND_FUNC(OnEvent));

		renderer = new rRenderer(window);

		OnCreate();
	}

	Application::~Application() {
		OnClosure();

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
		dispatcher.Dispatch<QuitEvent>(EMBER_BIND_FUNC(OnClose));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(OnResize));
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
}
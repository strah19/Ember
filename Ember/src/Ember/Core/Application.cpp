#include "EmberPch.h"
#include "Application.h"

namespace ember {
	Application::Application(const std::string& name, uint32_t width, uint32_t height) 
		: is_running(true) {
		properties = std::make_shared<WindowProperties>(name, width, height);
		window = std::make_shared<WindowsWindow>(properties.get());
		events = std::make_shared<Input>();
		event_handler = std::make_shared<EventHandler>(window.get(), events.get());
		event_handler->SetEventCallback(EMBER_BIND_EVENT(OnEvent));

		renderer = std::make_shared<Renderer2D>(window.get());
	}

	void Application::Run() {
		OnCreate();
		while (is_running) {
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
		is_running = false;
		return true;
	}

	bool Application::OnResize(const ResizeEvent& event) {
		window->Properties()->width = event.w;
		window->Properties()->height = event.h;
		return true;
	}
}
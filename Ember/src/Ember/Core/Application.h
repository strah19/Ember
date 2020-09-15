#ifndef APPLICATION_H
#define APPLICATION_H

#include "Platform/Window/WindowsWindow.h"

#include "Event/Event.h"
#include "Event/WindowEvents.h"
#include "Event/MouseEvents.h"
#include "Event/EventHandler.h"

#include "../PlatformDetection.h"
#include "Renderer/2DRenderer.h"

int main(int argc, char** argv);

namespace ember {
	class Application {
	public:
		Application(const std::string& name = "Game Engine", uint32_t width = 1280, uint32_t height = 720);
		virtual ~Application() = default;

		void Run();

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUserUpdate() { }
		virtual void OnCreate() { }

	protected:
		bool is_running;

#ifdef EMBER_WINDOW_PLATFORM
		std::shared_ptr<WindowsWindow> window;
#endif // EMBER_WINDOW_PLATFORM
		std::shared_ptr<EventHandler> event_handler;
		std::shared_ptr<Input> events;
		std::shared_ptr<Renderer2D> renderer;
	private:
		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);

		std::shared_ptr<WindowProperties> properties;
	};
}

#endif // !APPLICATION_H

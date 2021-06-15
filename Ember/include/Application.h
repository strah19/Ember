#ifndef APPLICATION_H
#define APPLICATION_H

#include "Application.h"

#include "Ember.h"
#include "EventHandler.h"
#include "Renderer.h"
#include "Window.h"
#include "Logger.h"

namespace Ember {
	enum AppFlags {
		NONE = 0x01,
		FULL_SCREEN = 0x02,
		OPENGL = 0x04
	};

	class Application {
	public:
		void Initialize(const std::string& name = "EmberApp", uint32_t width = 1280, uint32_t height = 720, AppFlags flags = AppFlags::NONE);

		virtual ~Application();

		void Run();

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUserUpdate() { }
		virtual void OnCreate() { }

		Window* GetWindow() { return window; }
		rRenderer* GetRenderer() { return renderer; }
	protected:
		Window* window = nullptr;
		EventHandler* event_handler = nullptr;
		rRenderer* renderer = nullptr;
		WindowProperties* properties = nullptr;
	private:
		void OnClose(const QuitEvent& event);
		void OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);
	};
}

#endif // !APPLICATION_H

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Application.h"

#include "Ember.h"
#include "EventHandler.h"
#include "Renderer.h"
#include "Window.h"

namespace Ember {
	class Application {
	public:
		void Initialize(const std::string& name = "Ember", bool full_screen = false, uint32_t width = 1280, uint32_t height = 720);

		virtual ~Application();

		void Run();

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUserUpdate() { }
		virtual void OnCreate() { }
		virtual void OnClosure() { }

		Window* GetWindow() { return window; }
		Events* GetEvents() { return events; }
		rRenderer* GetRenderer() { return renderer; }
	protected:
		Window* window = nullptr;
		EventHandler* event_handler = nullptr;
		Events* events = nullptr;
		rRenderer* renderer = nullptr;
		WindowProperties* properties = nullptr;
	private:
		bool OnClose(const QuitEvent& event);
		bool OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);
	};
}

#endif // !APPLICATION_H

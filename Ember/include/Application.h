#ifndef APPLICATION_H
#define APPLICATION_H

#include "Application.h"

#include "Ember.h"
#include "EventHandler.h"
#include "Window.h"
#include "Logger.h"

namespace Ember {
	enum AppFlags {
		NONE = 0x01,
		FULL_SCREEN = 0x02,
		OPENGL_CUSTOM_VERSION = 0x04
	};

	class Application {
	public:
		void Initialize(const std::string& name = "EmberApp", uint32_t width = 1280, uint32_t height = 720, AppFlags flags = AppFlags::NONE);

		virtual ~Application();

		void Run();

		virtual void UserDefEvent(Event& event) { }
		virtual void OnUserUpdate(float delta) { }
		virtual void OnCreate() { }

		Window* GetWindow() { return window; }
	protected:
		Window* window = nullptr;
		EventHandler* event_handler = nullptr;
		WindowProperties* properties = nullptr;

		uint32_t opengl_minor_version = 0;
		uint32_t opengl_major_version = 0;
	private:
		void OnClose(const QuitEvent& event);
		void OnResize(const ResizeEvent& event);
		void OnEvent(Event& event);
	};
}

#endif // !APPLICATION_H

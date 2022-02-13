#include "Application.h"
#include "OpenGLWindow.h"
#include "ImGuiLayer.h"
#include "RendererCommands.h"
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>

namespace Ember {
	void Application::Initialize(const std::string& name, uint32_t width, uint32_t height, AppFlags flags) {
		Ember::LogImpl::Init();
		properties = new WindowProperties(name, width, height);

		properties->full_screen = (flags & AppFlags::FULL_SCREEN) ? true : false;
		window = Window::CreateOpenGLWindow(properties, (flags & OPENGL_CUSTOM_VERSION) ? opengl_major_version : 0, (flags & OPENGL_CUSTOM_VERSION) ? opengl_minor_version : 0);

		event_handler = new EventHandler(window);
		event_handler->SetEventCallback(EMBER_BIND_FUNC(OnEvent));
		renderer = new Renderer;

		OnCreate();
	}

	void Application::PushLayer(Layer* layer) {
		layers.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		layers.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application() {
		delete properties;
		delete window;
		delete event_handler;
		delete renderer;
	}

	void Application::Run() {
		float delta = 0;

		while (window->IsRunning()) {
			uint64_t now = SDL_GetTicks();
			if (delta != 0) {
				event_handler->Update();

				for (Layer* layer : layers)
					layer->OnUpdate(delta);

				if (imgui)
					imgui->Begin();
				OnGuiUpdate();
				for (Layer* layer : layers)
					layer->UpdateGui();
				if (imgui)
					imgui->End();
				window->Update();

				OnUserUpdate(delta);
			}
			delta = (float) (SDL_GetTicks() - now) / 1000.0f;
		}
	}

	void Application::OnEvent(Event& event) {
		EventDispatcher dispatcher(&event);

		UserDefEvent(event);
		for (auto it = layers.rbegin(); it != layers.rend(); ++it)
			(*it)->UserDefEvent(event);

		dispatcher.Dispatch<QuitEvent>(EMBER_BIND_FUNC(OnClose));
		dispatcher.Dispatch<ResizeEvent>(EMBER_BIND_FUNC(OnResize));
	}

	void Application::OnClose(const QuitEvent& event) {
		window->Quit();
	}

	void Application::OnResize(const ResizeEvent& event) {
		window->Properties()->width = event.w;
		window->Properties()->height = event.h;
		Ember::RendererCommand::SetViewport(0, 0, window->Properties()->width, window->Properties()->height);
	}
}
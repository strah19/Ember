#include "Application.h"

#include "OpenGLWindow.h"
#include <examples/imgui_impl_opengl3.cpp>
#include <examples/imgui_impl_sdl.cpp>

namespace Ember {
	void Application::Initialize(const std::string& name, uint32_t width, uint32_t height, AppFlags flags) {
		Ember::LogImpl::Init();
		properties = new WindowProperties(name, width, height);

		properties->full_screen = (flags & AppFlags::FULL_SCREEN) ? true : false;
		window = Window::CreateOpenGLWindow(properties, (flags & OPENGL_CUSTOM_VERSION) ? opengl_major_version : 0, (flags & OPENGL_CUSTOM_VERSION) ? opengl_minor_version : 0);

		event_handler = new EventHandler(window);
		event_handler->SetEventCallback(EMBER_BIND_FUNC(OnEvent));
		if (!(flags & AppFlags::CUSTOM_RENDERER))
			renderer = new Renderer;
		else
			EMBER_LOG_WARNING("You are not using Embers defined renderer...");

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();

		OpenGLWindow* w = static_cast<OpenGLWindow*>(window);
		ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(w->GetNativeWindow()), w->Context());
		ImGui_ImplOpenGL3_Init("#version 450");

		OnCreate();
	}

	Application::~Application() {
		delete properties;
		delete window;
		delete event_handler;
		delete renderer;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::Run() {
		uint64_t now = SDL_GetPerformanceCounter();
		uint64_t last = 0;
		float delta = 0;

		while (window->IsRunning()) {

			ImGui_ImplSDL2_ProcessEvent(event_handler->NativeEvent());
			event_handler->Update();

			last = now;
			now = SDL_GetPerformanceCounter();

			delta = (float)((now - last) * 1000 / (float)SDL_GetPerformanceFrequency());

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(window->GetNativeWindow()));
			ImGui::NewFrame();

			{
				ImGui::Begin("Controls", NULL);

				ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
				ImGui::Text("%s", SDL_GetPlatform());
				ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
				ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			window->Update();
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
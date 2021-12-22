#include "ImGuiLayer.h"
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>
#include "Application.h"

namespace Ember {
	void ImGuiLayer::Begin() {
		ImGui_ImplSDL2_ProcessEvent(event_handler->NativeEvent());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(window->GetNativeWindow()));
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable && (flags & GuiFlags::GUI_DOCKER))
		{
			auto gl = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(static_cast<SDL_Window*>(window->GetNativeWindow()), gl);
		}
	}

	ImGuiLayer::ImGuiLayer(Window* window, EventHandler* event_handler, int flags) : window(static_cast<OpenGLWindow*>(window)), event_handler(event_handler), flags(flags), Layer("ImGui Layer") {

	}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (flags & GuiFlags::GUI_DOCKER) {
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		}

		ImGui::StyleColorsDark();

		ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(window->GetNativeWindow()), window->Context());
		ImGui_ImplOpenGL3_Init("#version 450");		//Have sure this is not hard coded!
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(float delta) {

	}

	void ImGuiLayer::UserDefEvent(Event& event) {

	}
}
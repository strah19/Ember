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
	}

	ImGuiLayer::ImGuiLayer(Window* window, EventHandler* event_handler) : window(static_cast<OpenGLWindow*>(window)), event_handler(event_handler), Layer("ImGui Layer") {

	}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
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
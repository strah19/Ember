#include "ImGuiLayer.h"
#include "Application.h"

namespace Ember {
	void ImGuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
		SDL_Window* win_data = static_cast<SDL_Window*>(window->GetNativeWindow());
		ImGui_ImplSDL2_NewFrame(win_data);
        ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	ImGuiLayer::ImGuiLayer(Window* window, SDL_GLContext* context) : Layer("ImGui Layer") {
		this->window = window;
		this->context = context;
	}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		SDL_Window* win_data = static_cast<SDL_Window*>(window->GetNativeWindow());

		ImGui_ImplSDL2_InitForOpenGL(win_data, context);
		ImGui_ImplOpenGL3_Init("#version 450");
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
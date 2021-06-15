#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <imgui.h>
#include <memory>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>
#include "Layer.h"
#include "Window.h"

namespace Ember {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(Window* window, SDL_GLContext* context);
		
		virtual void OnAttach();
		void OnDetach();

		void OnUpdate(float delta);
		void UserDefEvent(Event& event);
		void UpdateGui() { }

		void Begin();
		void End();
	private:
		Window* window;
		SDL_GLContext* context;
	};
}

#endif // !IMGUI_LAYER_H

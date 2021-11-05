#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <cstdint>
#include "Window.h"
#include "Layer.h"
#include "OpenGLWindow.h"
#include "EventHandler.h"

namespace Ember {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(Window* window, EventHandler* event_handler);

		virtual void OnAttach();
		void OnDetach();

		void OnUpdate(float delta);
		void UserDefEvent(Event& event);
		void UpdateGui() { }

		void Begin();
		void End();
	private:
		OpenGLWindow* window = nullptr;
		EventHandler* event_handler = nullptr;
	};
}

#endif // !IMGUI_LAYER_H

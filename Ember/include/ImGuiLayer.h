#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <cstdint>
#include "Window.h"
#include "Layer.h"
#include "OpenGLWindow.h"
#include "EventHandler.h"

namespace Ember {
	enum GuiFlags {
		GUI_NONE = 0x00,
		GUI_DOCKER = 0x01
	};

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer(Window* window, EventHandler* event_handler, int flags = GuiFlags::GUI_NONE);

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
		int flags = GuiFlags::GUI_NONE;
	};
}

#endif // !IMGUI_LAYER_H

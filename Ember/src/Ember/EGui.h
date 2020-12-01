#ifndef EGUI_H
#define E_GUI_H

#include "Ember.h"

namespace Ember {
	static bool CheckApplication(Ember::Application* app);

	struct SerializerInfo {
		Rect position = { 0, 0, 0, 0 };
		bool collapsed = false;
	};

	struct EGuiWindow {
		SerializerInfo window_info;
		size_t window_id;
		bool window_configured = false;
	};

	class EGuiContext {
	public:
		EGuiWindow* GetCurrentWindow() { 
			if(!gui_windows.empty())
				return &gui_windows[current_window_index]; 
			
			return nullptr;
		}
		void PassApplication(Application* app);
		void IncrementWindow() { current_window_index++; }
		void ResetWindows() { current_window_index = 0; }
		size_t CurrentWindowIndex() { return current_window_index; }

		void AddToList(const SerializerInfo& data);
	private:
		Application* application = nullptr;;

		size_t max_window_ids_found = 0;
		size_t current_window_index = 0;
		std::vector<EGuiWindow> gui_windows;
	};

	enum class InterfaceErrors {
		ApplicationFail, None
	};

	class EGuiInterface {
	public:
		InterfaceErrors CheckErrors() { return interface_errors; }
		friend void SetError(EGuiInterface& interfaces, InterfaceErrors error);
	private:
		InterfaceErrors interface_errors;
	};

	SerializerInfo ReadDataWindows();
	SerializerInfo WriteWindowData();

	void SetError(EGuiInterface& interfaces, InterfaceErrors interface_error);
	void CreateGuiInterfaceInApp(EGuiInterface& gui_interface, Application* app);

	void StartFrame();
	void Begin(const std::string& title);
	void End();
}

#endif // !EGUI_H

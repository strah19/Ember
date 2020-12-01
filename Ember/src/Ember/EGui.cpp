#include "EGui.h"

Ember::EGuiContext* context = nullptr;
std::string file_config_name = "config.txt";
Ember::File file(file_config_name.c_str());

#define SERIALIZER_SIZE 4

namespace Ember {
	static bool CheckApplication(Ember::Application* app) {
		return(app != nullptr);
	}

	void SetError(EGuiInterface& interfaces, InterfaceErrors error) {
		interfaces.interface_errors = error;
	}

	void EGuiContext::PassApplication(Application* app) {
		if (CheckApplication(app)) 
			application = app; 
	}

	void CreateGuiInterfaceInApp(EGuiInterface& gui_interface, Application* app) {
		if (!CheckApplication(app)) 
			SetError(gui_interface, InterfaceErrors::ApplicationFail);
		else {
			context = new EGuiContext;
			SetError(gui_interface, InterfaceErrors::None);
			context->PassApplication(app);
		}
	}

	void EGuiContext::AddToList(const SerializerInfo& data) {
		gui_windows.push_back({ data, context->CurrentWindowIndex() });
	}

	void StartFrame() {
		context->ResetWindows();
	}

	SerializerInfo ReadDataWindows() {
		static int current_position = 0;
		SerializerInfo info;

		std::cout << file.ReadAll() << std::endl;

		return info;
	}

	void Begin(const std::string& title) {
		EGuiWindow* current_window = context->GetCurrentWindow();

		if (current_window == nullptr) {
		}

		context->IncrementWindow();
	}

	void End() {

	}
}


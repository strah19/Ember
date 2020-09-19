#include "Core/Ember.h"
#include <array>

class Sandbox : public ember::Application {
public:
	Sandbox() 
		: logger({ 100, 100, 300, 300 }, renderer, events) {
		logger.SetPrefixSuffix("Command: ", " This is the suffix!");

		logger.AddToLogQueue("In Sandbox Constructor!");
	}

	void OnUserUpdate() {
		renderer->Clear({ 100, 120, 190, 255 });
		ember::BeginFrame();

		logger.Render();

		ember::BeginGui({ 100, 200 });
		ember::BeginGui();

		renderer->Show();
	}

	void OnCreate() {
		if (ember::InitializeEmberGui(renderer, events) == ember::EmberGuiError::RendererEventsNull)
			window->Quit();
	}

	void UserDefEvent(ember::Event& event) {
		
	}
	ember::GuiLog logger;
};

int main(int argc, char** argv)
{
	Sandbox sandbox;

	sandbox.Run();

	return 0;  
}
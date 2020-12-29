#include "StartUp/Application.h"
#include "Gui/GuiObjectFramework.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 
		scale.CalculatePercents({ 0, 0, 100, 100 }, { properties->width, properties->height });
	}

	void OnUserUpdate() {
		window->Update();
		window->SetResizeable(true);

		Ember::Rect pos = scale.Update({ properties->width, properties->height });

		renderer->Clear(background_color);
		
		renderer->Rectangle(pos, { 255, 0, 0, 255 });

		renderer->Show();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	Ember::Scaleable scale;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();
	return 0;
}
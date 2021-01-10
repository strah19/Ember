#include "Core/Application.h"
#include "Assets/Texture.h"
#include "Animation/Spritesheet.h"
#include "Animation/Animation.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		texture.Initialize("animation.png", renderer);
		spritesheet.Initialize(texture, 1, 28);
		animation_source.Initialize(spritesheet, 28);
	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		animation.Update(animation_source, 80);
		animation_source.DrawCurrentFrame(animation.CurrentFrame(), Ember::Rect({ 0, 300, 300, 300 }));

		renderer->Show();
	}

	bool Keyboard(Ember::KeyboardEvents& keyboard) {
		return false;
	}

	void UserDefEvent(Ember::Event& event) { 
		Ember::EventDispatcher dispatch(&event);
		dispatch.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(Keyboard));
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	Ember::SpriteSheet spritesheet;
	Ember::Texture texture;

	Ember::Animation animation;
	Ember::SpriteSheetToAnimation animation_source;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}
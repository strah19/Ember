#include "Core/Application.h"
#include "Components/Animation/Animation.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		texture.Initialize("anim.png", renderer);
		spritesheet.Initialize(texture, 1, 7);
		anim_textures.Initialize(spritesheet, 7);
		anim_textures.SetIterator(Ember::SpriteSheetIterators::HORIZONTAL);
	}

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();
		window->SetResizeable(true);

		renderer->Clear(background_color);
		
		animation.Update(anim_textures, 80);
		anim_textures.DrawCurrentFrame(animation.CurrentFrame(), { 0, 0, 200, 200 });

		renderer->Show();
	}

	bool Keyboard(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == Ember::EmberKeyCode::Escape) {
			window->Quit();
			return true;
		}
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

	Ember::SpriteSheetToAnimation anim_textures;
	Ember::Animation animation;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}
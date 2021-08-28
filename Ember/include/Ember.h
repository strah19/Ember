#ifndef EMBER_H
#define EMBER_H

/*
Example code:

#include "Core/Application.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	}

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		renderer->Show();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}
*/

#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Vector.h" 

#define EMBER_BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ember {
	struct Rect {
		union {
			struct {
				int x, y, w, h;
			};
			struct {
				IVec2 pos, size;
			};
			struct {
				SDL_Rect rect;
			};
		};

		Rect() = default;
		~Rect() = default;

		Rect(const Rect& rect) { this->rect = rect.rect; }
		Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) { }

		void operator=(const Rect& rect) { this->rect = rect.rect; }
	};

	struct FRect {
		union {
			struct {
				float x, y, w, h;
			};
			struct {
				Vec2 pos, size;
			};
			struct {
				SDL_FRect rect;
			};
		};

		FRect() = default;
		~FRect() = default;

		FRect(const FRect& rect) { this->rect = rect.rect; }
		FRect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) { }

		void operator=(const FRect& rect) { this->rect = rect.rect; }
	};

	struct Color {
		union {
			struct {
				Uint8 r, g, b, a;
			};
			struct {
				SDL_Color color;
			};
		};

		Color() = default;
		~Color() = default;

		Color(const Color& color) { this->color = color.color; }
		Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) { }

		void operator=(const Color& color) { this->color = color.color; }
	};

	SDL_DisplayMode GetDisplay();
	SDL_bool ConvertToSDLBool(bool value);
	void CheckVersion();
}

#endif // !EMBER_H

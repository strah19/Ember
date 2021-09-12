#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "OrthoCamera.h"
#include "OrthoCameraController.h"
#include "PerspectiveCameraController.h"
#include "Texture.h"
#include "OpenGLWindow.h"
#include "TextureAtlas.h"
#include "Font.h"
#include "FrameBuffer.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		cam = Ember::OrthoCamera(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
		cam.SetPosition({ 0, 0, 0 });

		player = { 200, 200, 0.0f, 0.0f, 100.0f, 0 };
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
	}

	void wrap(float ix, float iy, float& ox, float& oy, float width, float height) {
		ox = ix;
		oy = iy;

		if (ix + width > SCREEN_WIDTH) ox = -(width - ((ix + width) - SCREEN_WIDTH));
		if (ix < 0) ox = SCREEN_WIDTH - -(ix);
		if (iy + height > SCREEN_HEIGHT) { oy = 0; oy -= height - ((iy + height) - SCREEN_HEIGHT); }
		if (iy < 0) oy = SCREEN_HEIGHT - -(iy);
	}

	void OnUserUpdate(float delta) {
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::LeftArrow))
			player.angle += 3.0f;
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::RightArrow))
			player.angle -= 3.0f;
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::UpArrow)) {
			player.dx += sin(player.angle);
			player.dy += cos(player.angle);
		}

		player.x += player.dx;
		player.y += player.dy;

		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		Ember::Renderer::BeginScene(cam, Ember::RenderFlags::TopLeftCornerPos);
		Ember::Renderer::SetShaderToDefualt();

		Ember::Renderer::DrawRotatedQuad({ player.x, player.y, 0 }, player.angle, { 0, 0, 1 }, { player.size, player.size }, { 1, 0, 0, 1 });

		if (player.x > SCREEN_WIDTH || player.y > SCREEN_HEIGHT || player.x + player.size < 0 || player.y + player.size < 0)
			wrap(player.x, player.y, player.x, player.y, player.size, player.size); 
		if (player.x + player.size > SCREEN_WIDTH || player.y + player.size > SCREEN_HEIGHT || player.x < 0 || player.y < 0) {
			float nx, ny;
			wrap(player.x, player.y, nx, ny, player.size, player.size);
			Ember::Renderer::DrawRotatedQuad({ nx, ny, 0 }, player.angle, { 0, 0, 1 }, { player.size, player.size }, { 1, 1, 0, 1 });
		}

		Ember::Renderer::DrawTriangle({ 400, 400, 0 }, { 200, 200 }, { 1, 0, 1, 1 });

		Ember::Renderer::EndScene();
		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
	}
private:
	Ember::OrthoCamera cam;

	struct WorldObject {
		float x = 0.0f, y = 0.0f;
		float dx = 0.0f, dy = 0.0f;
		float size = 0.0f;
		float angle = 0.0f;
	};

	std::vector<WorldObject> asteroids;
	WorldObject player;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", SCREEN_WIDTH, SCREEN_HEIGHT);

	sandbox.Run();

	return 0;
}
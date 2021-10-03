#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "PerspectiveCameraController.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		camera = Ember::PerspectiveCameraController(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
	}

	void OnUserUpdate(float delta) {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		camera.Update();
		Ember::Renderer::BeginScene(camera.GetCamera());
		Ember::Renderer::SetShaderToDefualt();


		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if ((i + j) % 2 == 0)
					Ember::Renderer::DrawQuad({ i * 1, j * 1, -1 }, { 1, 1 }, { 1, 0, 0, 1 });
				else
					Ember::Renderer::DrawQuad({ i * 1, j * 1, -1 }, { 1, 1 }, { 1, 1, 0, 1 });

			}
		}

		Ember::Triangle::DrawTriangle({ -1, -1, 0 }, { 1, 1 }, { 0, 1, 1, 1 });

		Ember::Cube::DrawCube({ -2, -2, 0 }, { 1, 1, 1 }, { 1, 0, 1, 1 });

		Ember::Renderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		camera.OnEvent(event);
	}
private:
	Ember::PerspectiveCameraController camera;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", WINDOW_WIDTH, WINDOW_HEIGHT);

	sandbox.Run();

	return 0;
}


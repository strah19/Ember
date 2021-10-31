#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "PerspectiveCameraController.h"
#include "Geometry.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		Ember::RendererCommand::Init();
		Ember::RendererCommand::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		camera = Ember::PerspectiveCameraController(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

		q = Ember::Quad::GetQuad();
	}

	virtual ~Sandbox() {
	}

	void OnUserUpdate(float delta) {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		camera.Update();
		renderer->BeginScene(&camera.GetCamera());

		q->Update(renderer->GetGraphicsDevice());
		q->DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 0, 0, 1 });

		renderer->EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		camera.OnEvent(event);
	}
private:
	Ember::PerspectiveCameraController camera;
	Ember::Quad* q;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", WINDOW_WIDTH, WINDOW_HEIGHT);

	sandbox.Run();

	return 0;
}


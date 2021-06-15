#include "Application.h"
#include "OpenRenderer.h"
#include "RendererCommands.h"
#include "OrthoCamera.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::OpenGLRenderer::Init();

		cam = Ember::OrthoCamera(0, 1280, 720, 0);
		cam.SetPosition({ 0, 0, 0 });
	}

	virtual ~Sandbox() {
		Ember::OpenGLRenderer::Destroy();
	}

	void OnUserUpdate() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(1.0, 0.0, 0.0, 1.0);

		Ember::OpenGLRenderer::BeginScene(cam);

		Ember::OpenGLRenderer::DrawQuad({ 10, 10, 0 }, { 200, 200 }, { 0, 1, 0, 1 });
		Ember::OpenGLRenderer::MakeCommand();

		Ember::OpenGLRenderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	Ember::Camera cam;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720, Ember::AppFlags::OPENGL);

	sandbox.Run();

	return 0;
}
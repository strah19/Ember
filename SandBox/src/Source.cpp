#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "OrthoCamera.h"
#include "OrthoCameraController.h"
#include "PerspectiveCameraController.h"
#include "Texture.h"
#include "OpenGLWindow.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, 1280, 720);

		cam = Ember::PerspectiveCameraController(glm::vec2(1280, 720));
		window->SetWindowIcon("tex.png");

		t = new Ember::Texture("tex2.png");
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
	}

	void OnUserUpdate() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(1.0, 0.0, 0.0, 1.0);

		cam.Update();
		Ember::Renderer::BeginScene(cam.GetCamera());
		Ember::Renderer::SetShaderToDefualt();

		Ember::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, t);

		Ember::Renderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		cam.OnEvent(event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	Ember::PerspectiveCameraController cam;
	Ember::Texture* t;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720, Ember::AppFlags::OPENGL);

	sandbox.Run();

	return 0;
}
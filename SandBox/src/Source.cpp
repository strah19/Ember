#include "Application.h"
#include "OpenGLRenderer.h"
#include "OpenGLRendererCommands.h"
#include "OpenGLOrthoCamera.h"
#include "OpenGLOrthoCameraController.h"
#include "OpenGLPerspectiveCameraController.h"
#include "OpenGLWindow.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		EmberGL::RendererCommand::Init();
		EmberGL::Renderer::Init();
		EmberGL::RendererCommand::SetViewport(0, 0, 1280, 720);

		cam = EmberGL::PerspectiveCameraController(glm::vec2(1280, 720));
	}

	virtual ~Sandbox() {
		EmberGL::Renderer::Destroy();
	}

	void OnUserUpdate() {
		EmberGL::RendererCommand::Clear();
		EmberGL::RendererCommand::SetClearColor(1.0, 0.0, 0.0, 1.0);

		cam.Update();
		EmberGL::Renderer::BeginScene(cam.GetCamera());
		EmberGL::Renderer::SetShaderToDefualt();

		EmberGL::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 1, 0, 1 });

		EmberGL::Renderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		cam.OnEvent(event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	EmberGL::PerspectiveCameraController cam;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720, Ember::AppFlags::OPENGL);

	sandbox.Run();

	return 0;
}
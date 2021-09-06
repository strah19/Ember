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

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, 1280, 720);

		cam = Ember::OrthoCameraController(glm::vec2(1280, 720));

		texture = new Ember::Texture("tex3.png");
		texture2 = new Ember::Texture("tex2.png");
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
	}

	void OnUserUpdate() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		cam.Update();
		Ember::Renderer::BeginScene(cam.GetCamera());

		Ember::Renderer::SetShaderToDefualt();

		Ember::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, texture);
		Ember::Renderer::DrawQuad({ 1, 0, 0 }, { 1, 1 }, texture2);

		Ember::Renderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		cam.OnEvent(event);
	}
private:
	Ember::OrthoCameraController cam;
	Ember::Texture* texture;
	Ember::Texture* texture2;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720);

	sandbox.Run();

	return 0;
}
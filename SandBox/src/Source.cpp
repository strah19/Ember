#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "OrthoCamera.h"
#include "OrthoCameraController.h"
#include "PerspectiveCameraController.h"
#include "Texture.h"
#include "OpenGLWindow.h"
#include "TextureAtlas.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, 1280, 720);

		cam = Ember::PerspectiveCameraController(glm::vec2(1280, 720));
		window->SetWindowIcon("tex.png");

		texture = new Ember::Texture("tex2.png");

		atlas.Init(texture, 10, 10);

		Ember::RandomAccessTextureAtlas ra;
		ra.Init(texture, "data.txt");
		//glm::vec2 data[] = { glm::vec2({0.0, 0.0}), glm::vec2({1.0, 0.0}), glm::vec2({0.0, 1.0}), glm::vec2({1.0, 1.0}) };
		//ra.AddTile("Tree", data);
		ra.LogEntries();
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

		Ember::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, texture, atlas.GetTexCoords(0, 0, glm::vec2(6, 5)));

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
	Ember::Texture* texture;
	Ember::TextureAtlas atlas;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720, Ember::AppFlags::OPENGL);

	sandbox.Run();

	return 0;
}
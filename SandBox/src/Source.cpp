#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "OrthoCameraController.h"
#include "Geometry.h"
#include "FrameBuffer.h"
#include "Font.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		Ember::RendererCommand::Init();
		Ember::RendererCommand::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		camera = Ember::OrthoCameraController(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

		imgui = new Ember::ImGuiLayer(window, event_handler);
		PushLayer(imgui);

		font.Init("OpenSans-Regular.ttf", 20);
		text_shader = new Ember::Shader("shaders/text_shader.glsl");
		renderer->InitRendererShader(text_shader);
	}

	virtual ~Sandbox() { }
	    
	void OnUserUpdate(float delta) {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		camera.Update();

		renderer->BeginScene(&camera.GetCamera());
		renderer->SetShader(text_shader);

		Ember::Text::Renderer(renderer);
		Ember::Text::RenderText(&font, "What's up", { 0, 0 }, { 1, 1 }, { 1, 0, 1, 1 });

		renderer->EndScene();

		renderer->BeginScene(&camera.GetCamera());
		Ember::Quad::Renderer(renderer);
		Ember::Quad::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 0, 0, 1 });
		renderer->EndScene();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		camera.OnEvent(event);
	}

	void OnGuiUpdate() { }
private:
	Ember::OrthoCameraController camera;
	Ember::Font font;
	Ember::Shader* text_shader;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", WINDOW_WIDTH, WINDOW_HEIGHT);

	sandbox.Run();

	return 0;
}


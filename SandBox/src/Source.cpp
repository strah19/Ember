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
		window->SetWindowIcon("tex.png");

		texture = new Ember::Texture("tex2.png");

		atlas.Init(texture, 10, 10);

		Ember::RandomAccessTextureAtlas ra;
		ra.Init(texture, "data.txt");
		//glm::vec2 data[] = { glm::vec2({0.0, 0.0}), glm::vec2({1.0, 0.0}), glm::vec2({0.0, 1.0}), glm::vec2({1.0, 1.0}) };
		//ra.AddTile("Tree", data);
		ra.LogEntries();

		material_test_shader.Init("shaders/material_shader.glsl");
		ssbo = new Ember::ShaderStorageBuffer(sizeof(glm::vec4), 1);

		text_shader.Init("shaders/text_shader.glsl");
		f.Init("font.ttf", 48);    
		EMBER_LOG_WARNING("%d", f.GetSizeOfText("Test!"));
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
		delete ssbo;
		delete texture;
	}

	void OnUserUpdate() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		cam.Update();
		Ember::Renderer::BeginScene(cam.GetCamera());
		//Ember::Renderer::SetShader(&material_test_shader);
		Ember::Renderer::SetShader(&text_shader);
		//Ember::Renderer::SetShaderToDefualt();
		ssbo->Bind();
		glm::vec4 color = { 0.0, 1.0, 0.0, 1.0 };
		ssbo->SetData((void*)&color, sizeof(glm::vec4), 0);
		ssbo->BindToBindPoint();
		//Ember::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, texture, atlas.GetTexCoords(0, 0, glm::vec2(6, 5)));
		Ember::Renderer::RenderText(&f, " !\"#$ % &\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", { 0, 0 }, { 0.2, 0.2 }, { 0.8, 0.5, 0.7, 1.0 });
		//Ember::Renderer::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 0, 0, 1 });
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
	Ember::TextureAtlas atlas;
	Ember::Shader material_test_shader;
	Ember::ShaderStorageBuffer* ssbo;
	Ember::Font f;
	Ember::Shader text_shader;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720);

	sandbox.Run();

	return 0;
}
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
#include "RandomNumberGenerator.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define MAX_SPEED 10.0f
#define MIN_ASTEROID_SIZE 20

struct WorldObject {
	float x = 0.0f, y = 0.0f;
	float dx = 0.0f, dy = 0.0f;
	float size = 0.0f;
	float angle = 0.0f;
	bool alive = true;
};

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		cam = Ember::OrthoCamera(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
		cam.SetPosition({ 0, 0, 0 });

		player = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, 0.0f, 5.0f, 0 };

		ship_model.push_back({ 0.0f, -5.5f });
		ship_model.push_back({ -2.5f, 2.5f });
		ship_model.push_back({ 2.5f, 2.5f });

		srand(time(NULL));

		int verts = 20;
		for (int i = 0; i < verts; i++) {
			float noise = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;
			asteroid_model.push_back({ noise * sinf(((float)i / (float)verts) * 6.28318f),
				noise * cosf(((float)i / (float)verts) * 6.28318f) });
		}

		text_shader.Init("shaders/text_shader.glsl");
		Ember::Renderer::InitRendererShader(&text_shader);
		text.Init("font.ttf", 48);

		reset();
	}

	void reset() {
		asteroids.clear();

		for (int i = 0; i < level; i++) {
			asteroids.push_back({ (float)Ember::RandomGenerator::GenRandom(0, SCREEN_WIDTH), (float)Ember::RandomGenerator::GenRandom(0, SCREEN_HEIGHT),
				(float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f), (float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f), 50, 0 });
		}

		player.x = SCREEN_WIDTH / 2;
		player.y = SCREEN_HEIGHT / 2;
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

	bool circle_collision(float cx, float cy, float radius, float x, float y) {
		return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
	}

	float to_rad(float angle) {
		return (angle / 180) * 3.14159f;
	}

	void update() {
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::LeftArrow))
			player.angle += 3.0f;
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::RightArrow))
			player.angle -= 3.0f;
		if (Ember::KeyboardEvents::GetKeyboardState(Ember::EmberKeyCode::UpArrow)) {
			player.dx += sinf((player.angle / 180.f) * 3.14159f);
			player.dy += -cosf((player.angle / 180.f) * 3.14159f);

			if (player.dx > MAX_SPEED)
				player.dx = MAX_SPEED;
			else if (player.dx < -MAX_SPEED)
				player.dx = -MAX_SPEED;
			if (player.dy > MAX_SPEED)
				player.dy = MAX_SPEED;
			else if (player.dy < -MAX_SPEED)
				player.dy = -MAX_SPEED;
		}

		player.x += player.dx;
		player.y += player.dy;

		wrap(player.x, player.y, player.x, player.y, player.size, player.size);

		for (auto& asteroid : asteroids) {
			asteroid.x += asteroid.dx;
			asteroid.y += asteroid.dy;

			if (circle_collision(asteroid.x, asteroid.y, asteroid.size + player.size, player.x, player.y)) {
				reset();
				tries++;
			}
			wrap(asteroid.x, asteroid.y, asteroid.x, asteroid.y, asteroid.size, asteroid.size);
		}

		for (auto& bullet : bullets) {
			bullet.x += bullet.dx * MAX_SPEED;
			bullet.y += bullet.dy * MAX_SPEED;

			if (bullet.x < 0 || bullet.y < 0 || bullet.x > SCREEN_WIDTH || bullet.y > SCREEN_HEIGHT)
				bullet.alive = false;

			for (auto& asteroid : asteroids) {
				if (circle_collision(asteroid.x, asteroid.y, asteroid.size, bullet.x, bullet.y)) {
					asteroid.alive = false;
					bullet.alive = false;
					if (asteroid.size > MIN_ASTEROID_SIZE) {
						asteroids.push_back({ asteroid.x, asteroid.y, (float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f), (float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f),
							(float)((int)asteroid.size >> 1), 0.0f });
						asteroids.push_back({ asteroids.back().x, asteroids.back().y, (float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f), (float)Ember::RandomGenerator::GenRandom(-5.0f, 5.0f),
							asteroids.back().size, 0.0f });
					}
				}
			}
		}

		clean_up_objs(bullets);
		clean_up_objs(asteroids);

		if (asteroids.size() == 0) {
			level++;
			reset();
		}
	}

	void render() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		Ember::Renderer::BeginScene(cam, Ember::RenderFlags::TopLeftCornerPos);
		Ember::Renderer::SetShaderToDefualt();

		draw_wireframe(ship_model, player.x, player.y, player.angle, player.size, { 1, 1, 1, 1 });

		for (auto& asteroid : asteroids) 
			draw_wireframe(asteroid_model, asteroid.x, asteroid.y, asteroid.angle, asteroid.size, { 1, 1, 1, 1 }, 3);

		for (auto& bullet : bullets) {
			Ember::Renderer::DrawQuad({ bullet.x, bullet.y, 0 }, { 5, 5 }, { 1, 1, 1, 1 });
		}

		Ember::Renderer::EndScene();

		Ember::Renderer::BeginScene(cam, Ember::RenderFlags::TopLeftCornerPos);
		Ember::Renderer::SetShader(&text_shader);
		Ember::Renderer::RenderText(&text, std::to_string(level), { 0, 600 }, { 2, 2 }, { 1, 1, 1, 1 });
		Ember::Renderer::RenderText(&text, std::to_string(tries), { 0, 400 }, { 2, 2 }, { 1, 1, 1, 1 });

		Ember::Renderer::EndScene();
	}

	void OnUserUpdate(float delta) {
		if (!paused) update();

		render();

		window->Update();
	}

	void clean_up_objs(std::vector<WorldObject>& world_objs) {
		for (int i = 0; i < world_objs.size(); i++)
			if (!world_objs[i].alive)
				world_objs.erase(world_objs.begin() + i);
	}

	void keyboard_event(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == Ember::EmberKeyCode::Return && keyboard.pressed) {
			player.x = Ember::RandomGenerator::GenRandom(0, SCREEN_WIDTH);
			player.y = Ember::RandomGenerator::GenRandom(0, SCREEN_HEIGHT);
		}
		else if (keyboard.scancode == Ember::EmberKeyCode::Space && keyboard.pressed) {
			bullets.push_back({ player.x, player.y, sinf((player.angle / 180.f) * 3.14159f), -cosf((player.angle / 180.f) * 3.14159f), 1, player.angle });
		}
		else if (keyboard.scancode == Ember::EmberKeyCode::LeftAlt && keyboard.pressed) {
			EMBER_LOG("-------------------new entry-------------------");
			for (auto& a : asteroids) {
				EMBER_LOG("position: %f, %f, dir: %f, %f, angle: %f, size: %f", a.x, a.y, a.dx, a.dy, a.angle, a.size);
			}
		}
		else if (keyboard.scancode == Ember::EmberKeyCode::P && keyboard.pressed) {
			paused = !paused;
		}
	}

	void draw_wireframe(const std::vector<glm::vec2>& coords, float x, float y, float r, float scale, const glm::vec4& color, float width = 1.0f) {
		std::vector<glm::vec2> transformed;
		transformed.resize(coords.size());

		for (int i = 0; i < (int) transformed.size(); i++) {
			transformed[i].x = (coords[i].x * cosf(to_rad(r))) - (coords[i].y * sinf(to_rad(r)));
			transformed[i].y = (coords[i].x * sinf(to_rad(r))) + (coords[i].y * cosf(to_rad(r)));
		}

		for (int i = 0; i < (int) transformed.size(); i++) {
			transformed[i].x = transformed[i].x * scale;
			transformed[i].y = transformed[i].y * scale;
		}

		for (int i = 0; i < (int) transformed.size(); i++) {
			transformed[i].x += x;
			transformed[i].y += y;
		}

		for (int i = 0; i < (int) transformed.size() + 1; i++) {
			int j = (i + 1);
			Ember::Renderer::DrawLine({ transformed[i % transformed.size()].x, transformed[i % transformed.size()].y }, 
				{ transformed[j % transformed.size()].x, transformed[j % transformed.size()].y }, color, width);
		}
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);

		dispatch.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(keyboard_event));
	}
private:
	Ember::OrthoCamera cam;

	Ember::Font text;
	Ember::Shader text_shader;
	std::vector<WorldObject> asteroids;
	std::vector<WorldObject> bullets;
	WorldObject player;
	std::vector<glm::vec2> ship_model;
	std::vector<glm::vec2> asteroid_model;

	uint32_t level = 1;
	uint32_t tries = 0;
	bool paused = false;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("Asteroids", SCREEN_WIDTH, SCREEN_HEIGHT);

	sandbox.Run();

	return 0;
}
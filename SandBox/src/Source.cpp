#include "Core/Ember.h"
#include <array>

struct Theme {
	std::unordered_map<std::string, ember::Color> colors;
};

void ParseTheme(Theme* theme, const std::string& file_path) {
	ember::File parser = ember::File(file_path.c_str()); 
	if (theme != nullptr) {
		int each_counter = 0;
		std::string current_color_id;
		parser.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter % 4 == 0) {
				current_color_id = word;
				theme->colors[current_color_id].a = 255;
				each_counter = 0;
			}
			else {
				switch (each_counter) {
				case 0:
					theme->colors[current_color_id].r = std::stoi(word);
					std::cout << theme->colors[current_color_id].r << std::endl;
					break;
				case 1:
					theme->colors[current_color_id].g = std::stoi(word);
					break;
				case 2:
					theme->colors[current_color_id].b = std::stoi(word);
					break;
				default:
					break;
				}
				each_counter++;
			}
		});
	}
}

struct LogGuiAttributes {
	LogGuiAttributes(std::shared_ptr<ember::Renderer2D> renderer, std::shared_ptr<ember::Input> input) 
		: button(input.get(), 0, 0, 0, 0), logger_font(renderer.get(), "res/Inconsolata-Regular.ttf", " ", 14, { 200, 200, 200, 255 }, 0, 0) {

	}
	int border_thickness = 2;
	ember::Rect command_start_position = { 0, 0, 100, 300 };
	int command_height = 25;

	bool moving = false;
	int px = 0, py = 0;
	ember::Button button;
	ember::Font logger_font;
};

struct LogTextColorDefs {
	ember::Color main_text_color = { 200, 200, 200, 255 };
	ember::Color latest_text_command_color = { 200, 100, 0, 255 };
};

struct LogColorInfo {
	ember::Color border_color = { 60, 60, 60, 255 };
	ember::Color main_color = { 80, 80, 80, 255 };
	LogTextColorDefs text_colors;
};

struct BackLog {
	static const int MAX_LOGGER_COUNT = 5;
	std::array<std::string, MAX_LOGGER_COUNT> string_logger;
	int current_count = 0;
};

struct Logger {
	LogColorInfo colors;
	LogGuiAttributes attribs;
	BackLog log;
	std::shared_ptr<ember::Renderer2D> renderer;
	std::shared_ptr<ember::Input> input;

	Logger(const ember::Rect& rect, std::shared_ptr<ember::Renderer2D> renderer, std::shared_ptr<ember::Input> input) 
		: attribs(renderer, input), renderer(renderer), input(input) {
		attribs.command_start_position = rect;
	}

	Logger() = default;
};

class Log {
public:
	Log(const Logger& log)
		: logger(log) {
	}
	Log() = default;

	void AddToLogQueue(const std::string& log) {
		if (logger.log.current_count > logger.log.MAX_LOGGER_COUNT - 1) {
			for (int i = 0; i < logger.log.MAX_LOGGER_COUNT - 1; i++) {
				logger.log.string_logger[i] = logger.log.string_logger[i + 1];
			}
			logger.log.string_logger[logger.log.MAX_LOGGER_COUNT - 1] = log;
		}
		else {
			logger.log.string_logger[logger.log.current_count] = log;

			logger.log.current_count++;
		}
	}

	void DrawText(const ember::IVec2& pos, const ember::Color& color, const std::string& string) {
		logger.attribs.logger_font.SetPosition(pos.x, pos.y);
		logger.attribs.logger_font.ChangeFont(string, color);
		logger.attribs.logger_font.Render();
	}

	bool LogGui() {
		SDL_Rect sdl_temp = logger.attribs.command_start_position.rect;
		sdl_temp.x -= logger.attribs.border_thickness;
		sdl_temp.y -= logger.attribs.border_thickness;
		sdl_temp.w += logger.attribs.border_thickness * 2;
		sdl_temp.h += logger.attribs.border_thickness * 2;

		SDL_RenderSetClipRect(logger.renderer->Renderer(), &sdl_temp);
		logger.renderer->Rectangle({ logger.attribs.command_start_position.x - logger.attribs.border_thickness,
		logger.attribs.command_start_position.y - logger.attribs.border_thickness,
		logger.attribs.command_start_position.w + logger.attribs.border_thickness * 2, logger.attribs.command_start_position.h + logger.attribs.border_thickness * 2 },
			logger.colors.border_color);

		logger.renderer->Rectangle({ logger.attribs.command_start_position.x, logger.attribs.command_start_position.y,
			logger.attribs.command_start_position.w,
			logger.attribs.command_start_position.h },
			logger.colors.main_color);

		logger.renderer->Line({ logger.attribs.command_start_position.x,
		logger.attribs.command_start_position.y + logger.attribs.command_height },
			{ logger.attribs.command_start_position.x + logger.attribs.command_start_position.w,  logger.attribs.command_start_position.y + logger.attribs.command_height },
			logger.colors.border_color);

		DrawText({ logger.attribs.command_start_position.x, logger.attribs.command_start_position.y }, logger.colors.text_colors.main_text_color, "Log");

		logger.attribs.button.UpdatePosition(logger.attribs.command_start_position.x, logger.attribs.command_start_position.y);
		logger.attribs.button.UpdateSize(logger.attribs.command_start_position.size);

		if (logger.attribs.button.Click(1)) {
			if (!logger.attribs.moving) {
				logger.attribs.moving = true;
				logger.attribs.px = logger.input->MousePosition().x - logger.attribs.command_start_position.x;
				logger.attribs.py = logger.input->MousePosition().y - logger.attribs.command_start_position.y;
			}
		}
		else if (logger.attribs.moving)
			logger.attribs.command_start_position.pos = ember::IVec2(logger.input->MousePosition().x - logger.attribs.px, logger.input->MousePosition().y - logger.attribs.py);
		if (!logger.input->Down())
			logger.attribs.moving = false;
		SDL_RenderSetClipRect(logger.renderer->Renderer(), NULL);

		return logger.attribs.moving;
	}

	void PrintLog(const std::string& signature) {
		int current_position = logger.attribs.command_start_position.y + (logger.log.current_count * logger.attribs.command_height);

		for (int i = 0; i < logger.log.current_count; i++) {
			DrawText({ logger.attribs.command_start_position.x, current_position },
				(i < logger.log.current_count - 1) ? logger.colors.text_colors.main_text_color : logger.colors.text_colors.latest_text_command_color, signature + logger.log.string_logger[i]);
			current_position -= logger.attribs.command_height;
		}
	}
private:
	Logger logger;
};

class Sandbox : public ember::Application {
	void OnUserUpdate() {
		renderer->Clear({ 100, 120, 190, 255 });

		log->LogGui();
		log->PrintLog("Command: ");

		renderer->Show();
	}

	void OnCreate() {
		log = std::make_shared<Log>(Logger({ 0, 0, 100, 200 }, renderer, events));
		log->AddToLogQueue("Starting program");
		log->AddToLogQueue("Created Window!");
		log->AddToLogQueue("Created Renderer!");
	}

	void UserDefEvent(ember::Event& event) {
		
	}
public:
	std::shared_ptr<ember::Input> In() { return events; }
	std::shared_ptr<ember::Renderer2D> Renderer() { return renderer; }
	std::shared_ptr<Log> log;
};

int main(int argc, char** argv)
{
	Sandbox sandbox;

	sandbox.Run();

	return 0;
}
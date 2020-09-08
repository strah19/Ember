#include "Core/Ember.h"

ember::WindowProperties properties("App", 960, 800);
ember::WindowsWindow window(&properties);
ember::Renderer2D renderer(&window);
ember::EventHandler events(&window);
ember::Input input(&events);

struct Theme {
	std::unordered_map<std::string, ember::Color> colors;
};

struct ThemeFormatParser {
	ember::File parser = ember::File("light-theme.txt");
	Theme* theme;

	void Parse() {
		int each_counter = 0;
		std::string current_color_id; 
		parser.DoEachWord([&](std::string& word, unsigned int counter) {
			if (counter % 4 == 0) {
				current_color_id = word;
				theme->colors[current_color_id].a = 255;
				each_counter = 0;
			}
			else {
				switch (each_counter)
				{
				case 0:
					theme->colors[current_color_id].r = std::stoi(word);
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
};

struct LogGuiAttributes {
	int border_thickness = 2;
	ember::Rect command_start_position = { properties.width / 2 - 150, properties.height / 2 - 100, 300, 200 };
	int command_height = 25;

	bool moving = false;
	int px = 0, py = 0;
	ember::Button button = ember::Button(&input, 0, 0, 0, 0);
	ember::Font logger_font = ember::Font(&renderer, "res/Inconsolata-Regular.ttf", " ", 14, { 200, 200, 200, 255 }, 0, 0);
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

	Logger(const ember::Rect& rect) {
		attribs.command_start_position = rect;
	}

	Logger() = default;
};

class Log {
public:
	Log(const ember::Rect& rect) {
		logger.attribs.command_start_position = rect;
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

	void LogGui() {
		SDL_Rect sdl_temp = logger.attribs.command_start_position.rect;
		sdl_temp.x -= logger.attribs.border_thickness;
		sdl_temp.y -= logger.attribs.border_thickness;
		sdl_temp.w += logger.attribs.border_thickness * 2;
		sdl_temp.h += logger.attribs.border_thickness * 2;

		SDL_RenderSetClipRect(renderer.Renderer(), &sdl_temp);
		renderer.Rectangle({ logger.attribs.command_start_position.x - logger.attribs.border_thickness,
		logger.attribs.command_start_position.y - logger.attribs.border_thickness,
		logger.attribs.command_start_position.w + logger.attribs.border_thickness * 2, logger.attribs.command_start_position.h + logger.attribs.border_thickness * 2 },
			logger.colors.border_color);

		renderer.Rectangle({ logger.attribs.command_start_position.x, logger.attribs.command_start_position.y,
			logger.attribs.command_start_position.w,
			logger.attribs.command_start_position.h },
			logger.colors.main_color);

		renderer.Line({ logger.attribs.command_start_position.x,
		logger.attribs.command_start_position.y + logger.attribs.command_height },
			{ logger.attribs.command_start_position.x + logger.attribs.command_start_position.w,  logger.attribs.command_start_position.y + logger.attribs.command_height },
			logger.colors.border_color);

		DrawText({ logger.attribs.command_start_position.x, logger.attribs.command_start_position.y }, logger.colors.text_colors.main_text_color, "Log");

		logger.attribs.button.UpdatePosition(logger.attribs.command_start_position.x, logger.attribs.command_start_position.y);
		logger.attribs.button.UpdateSize(logger.attribs.command_start_position.size);

		if (logger.attribs.button.Click(1)) {
			if (!logger.attribs.moving) {
				logger.attribs.moving = true;
				logger.attribs.px = input.MousePosition().x - logger.attribs.command_start_position.x;
				logger.attribs.py = input.MousePosition().y - logger.attribs.command_start_position.y;
			}
		}
		else if (logger.attribs.moving)
			logger.attribs.command_start_position.pos = ember::IVec2(input.MousePosition().x - logger.attribs.px, input.MousePosition().y - logger.attribs.py);
		if (!input.Down())
			logger.attribs.moving = false;
		SDL_RenderSetClipRect(renderer.Renderer(), NULL);
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

int main(int argc, char* argv[])
{
	Log log({ 100, 100, 200, 100 });
	Log log2;
	log.AddToLogQueue("Starting program");
	log.AddToLogQueue("Created Window!");
	log.AddToLogQueue("Created Renderer!");

	log2.AddToLogQueue("New Queue");

	events.ResizeWin();

	Theme t;
	ThemeFormatParser p;
	p.theme = &t;
	p.Parse();
	std::cout << (int) t.colors["border_color"].r << std::endl;

	while (window.IsRunning()) {
		events.Update();

		renderer.Clear({ 100, 120, 190, 255 });

		log.LogGui();
		log.PrintLog("Command: ");

		log2.LogGui();
		log2.PrintLog("Log2 Command: ");

		renderer.Show();
	}

	return 0;
}
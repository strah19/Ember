#ifndef LOG_H
#define LOG_H

#include "Event/EventHandler.h"
#include "Structures/Rect.h"
#include "Renderer/2DRenderer.h"
#include "Core/AssetLoaders/Assets.h"
#include "Button.h"
#include "Widgets.h"

#include <array>

namespace ember {
	struct LogTheme {
		Color main_text_color = { 200, 200, 200, 255 };
		Color latest_text_color = { 200, 100, 0, 255 };
		Color border_color = { 80, 80, 80, 255 };
		Color main_color = { 90, 90, 90, 255 };
		Color scroll_color = { 60, 60, 60, 255 };
	};

	struct Log {
		static const int MAX_LOGGER_HISTORY = 100;
		std::array<std::string, MAX_LOGGER_HISTORY> in_log_history;
		int current_index = 0;
	};

	class GuiLog {
	public:
		GuiLog(const Rect& rect, std::shared_ptr<Renderer2D> renderer, std::shared_ptr<Input> events);
		void Render();
		void SetPrefixSuffix(const std::string& prefix, const std::string suffix);
		Log Logger() { return log; }
		void AddToLogQueue(const std::string& add_to_log);
		void ClearLog(); 
		void ChangeTheme(LogTheme& theme) { log_theme = theme; }
	private:
		Log log;
		LogTheme log_theme;

		std::shared_ptr<Renderer2D> renderer;
		std::shared_ptr<Input> events;

		DragDataObject dragger_info;
		DragDataObject scroll_info;
		IVec2 scroll_after_position;
		IVec2 scroll_size;

		DragDataObject scroll_info_horz;
		IVec2 scroll_after_position_horz;
		IVec2 scroll_size_horz;
		Button scroll_horz;
		Rect scroll_position_horz;

		Button dragger;
		Button scroll;
		Font logger_font;

		int border_thickness;
		int individual_log_command_height;
		Rect position;
		Rect scroll_position;

		std::string prefix_of_command, suffix_of_command;
	private:
		void DrawText(const ember::IVec2& position, const ember::Color& color, const std::string& string);
		void DragCommand();
		void RenderText();
	};
}

#endif // !LOG_H

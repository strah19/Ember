#include "EmberPch.h"
#include "Log.h"

namespace ember {
	const int BUTTON_DRAG_ID = 1;

	void GuiLog::AddToLogQueue(const std::string& add_to_log) {
		if (log.current_index > log.MAX_LOGGER_HISTORY - 1) {
			for (int i = 0; i < log.MAX_LOGGER_HISTORY - 1; i++) 
				log.in_log_history[i] = log.in_log_history[i + 1];		
			log.in_log_history[log.MAX_LOGGER_HISTORY - 1] = add_to_log;
		}
		else {
			log.in_log_history[log.current_index] = add_to_log;
			log.current_index++;
		}
	}

	void GuiLog::ClearLog() {
		for (size_t i = 0; i < log.in_log_history.size(); i++)
			log.in_log_history[i] = " ";
	}

	GuiLog::GuiLog(const ember::Rect& rect, std::shared_ptr<ember::Renderer2D> renderer, std::shared_ptr<ember::Input> events)
		: renderer(renderer), events(events), dragger(events, 0, 0, 0, 0), logger_font(renderer, "res/Inconsolata-Regular.ttf", " ", 14, { 200, 200, 200, 255 }, 0, 0), position(rect),
		individual_log_command_height(25), scroll_size(15, 25), border_thickness(2), scroll_position({ rect.x + rect.w - scroll_size.x, rect.y, scroll_size.x, scroll_size.y }),
		scroll(events, scroll_position), scroll_size_horz(25, 15), scroll_position_horz({ rect.x, rect.y + rect.h - scroll_size_horz.y, scroll_size_horz.x, scroll_size_horz.y }),
		scroll_horz(events, scroll_position_horz) {
	}

	void GuiLog::SetPrefixSuffix(const std::string& prefix, const std::string suffix) {
		prefix_of_command = prefix;
		suffix_of_command = suffix;
	}

	void GuiLog::Render() {
		SDL_Rect sdl_borders = { position.x - border_thickness, position.y - border_thickness, position.w + border_thickness * 2, position.h + border_thickness * 2 };;
		SDL_RenderSetClipRect(renderer->Renderer(), &sdl_borders);

		renderer->Rectangle({ position.x - border_thickness, position.y - border_thickness, position.w + border_thickness * 2, position.h + border_thickness * 2 },
		log_theme.border_color);

		renderer->Rectangle({ position.x, position.y, position.w, position.h },
		log_theme.main_color);

		RenderText();
		renderer->Rectangle(scroll_position, log_theme.scroll_color);
		renderer->Rectangle(scroll_position_horz, log_theme.scroll_color);

		DragCommand();

		SDL_RenderSetClipRect(renderer->Renderer(), NULL);
	}

	void GuiLog::DrawText(const ember::IVec2& position, const ember::Color& color, const std::string& string) {
		logger_font.UnlockFont();
		logger_font.SetPosition(position.x, position.y);

		logger_font.ChangeFont(string, color);
		logger_font.Render();
	}

	void GuiLog::DragCommand() {
		dragger.UpdatePosition(position.x, position.y);
		dragger.UpdateSize(position.size);

		scroll.UpdatePosition(scroll_position.pos);
		scroll.UpdateSize(scroll_position.size);

		scroll_horz.UpdatePosition(scroll_position_horz.pos);
		scroll_horz.UpdateSize(scroll_position_horz.size);

		if (!scroll_info.CalculateNewPositionAfterDrag(scroll_position, events, scroll.Click(BUTTON_DRAG_ID), DragDataObject::DragMask::Axis_y) &&
			!scroll_info_horz.CalculateNewPositionAfterDrag(scroll_position_horz, events, scroll_horz.Click(BUTTON_DRAG_ID), DragDataObject::DragMask::Axis_x)) {
			if (dragger.Click(BUTTON_DRAG_ID)) {
				if (!dragger_info.is_moving) {
					dragger_info.is_moving = true;
					dragger_info.pre_x = events->MousePosition().x - position.x;
					dragger_info.pre_y = events->MousePosition().y - position.y;
					scroll_after_position = { scroll_position.x - position.x, scroll_position.y - position.y };
					scroll_after_position_horz = { scroll_position_horz.x - position.x, scroll_position_horz.y - position.y };
				}
			}
			else if (dragger_info.is_moving) {
				position.pos = ember::IVec2(events->MousePosition().x - dragger_info.pre_x, events->MousePosition().y - dragger_info.pre_y);
				scroll_position.pos = position.pos + scroll_after_position;
				scroll_position_horz.pos = position.pos + scroll_after_position_horz;
			}
			if (!events->Down())
				dragger_info.is_moving = false;
		}
		scroll_position.x = position.x + position.w - scroll_size.x;
		scroll_position_horz.y = position.y + position.h - scroll_size_horz.y;

		if (scroll_position.y < position.y)
			scroll_position.y = position.y;
		if (scroll_position.y + scroll_size.y > position.y + position.h)
			scroll_position.y = position.y + position.h - scroll_size.y;	

		if (scroll_position_horz.x < position.x)
			scroll_position_horz.x = position.x;
		if (scroll_position_horz.x + scroll_size_horz.x > position.x + position.w)
			scroll_position_horz.x = position.x + position.w - scroll_size_horz.x;
	}

	void GuiLog::RenderText() {
		int current_position = position.y + (log.current_index * individual_log_command_height) - (scroll_position.y - position.y) - individual_log_command_height;
		int current_horz_pos = position.x - (scroll_position_horz.x - position.x);
		for (int i = 0; i < log.current_index; i++) {
			DrawText({ current_horz_pos, current_position },
				(i < log.current_index - 1) ? log_theme.main_text_color : log_theme.latest_text_color,
				prefix_of_command + log.in_log_history[i] + suffix_of_command);
			current_position -= individual_log_command_height;
		}
	}
}
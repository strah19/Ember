#include "EmberPch.h"
#include "Widgets.h"
#include "Core/AssetLoaders/Assets.h"
namespace ember {
	static Widget widget;
	std::vector<Rect> positions;
	std::vector<DragDataObject> draggers;
	std::shared_ptr<Font> font;
	int widget_counter = 0;
	int loop_count = 0;
	IVec2 in_widget_position;

	const int CLICK_ID = 1;
	const int once_per_function_call = 2;
	const int start_position_info = 100;

	bool DragDataObject::CalculateNewPositionAfterDrag(Rect& position, std::shared_ptr<Input> events, bool clicked, DragMask mask) {
		if (clicked) {
			if (!is_moving) {
				is_moving = true;
				pre_x = events->MousePosition().x - position.x;
				pre_y = events->MousePosition().y - position.y;
			}
		}
		else if (is_moving) {
			if (mask == DragMask::Both)
				position.pos = ember::IVec2(events->MousePosition().x - pre_x, events->MousePosition().y - pre_y);
			else if (mask == DragMask::Axis_x)
				position.pos = ember::IVec2(events->MousePosition().x - pre_x, position.y);
			else if (mask == DragMask::Axis_y)
				position.pos = ember::IVec2(position.x, events->MousePosition().y - pre_y);
		}
		if (!events->Down())
			is_moving = false;

		return is_moving;
	}

	EmberGuiError InitializeEmberGui(std::shared_ptr<Renderer2D> renderer, std::shared_ptr<Input> events) {
		if (renderer != nullptr && events != nullptr) {
			widget.renderer = renderer;
			widget.events = events;
			widget.button = std::make_shared<Button>(widget.events, 0, 0, 0, 0);
			font = std::make_shared<Font>(renderer, "res/Roboto-Medium.ttf", " ", widget.theme.font_size, widget.theme.main_text_color, 0, 0);
		}
		else
			return EmberGuiError::RendererEventsNull;

		return EmberGuiError::None;
	}

	void SetTheme(WidgetTheme& theme) {
		widget.theme = theme;
	}

	void DragGuiObject() {
		widget.button->UpdatePosition(positions[widget_counter].pos);
		widget.button->UpdateSize(positions[widget_counter].size);

		draggers[widget_counter].CalculateNewPositionAfterDrag(positions[widget_counter], widget.events, widget.button->Click(CLICK_ID), DragDataObject::DragMask::Both);
	}

	void DrawBorderedRect(const Rect& pos) {
		widget.renderer->Rectangle({ pos.x - widget.theme.border_thickness, pos.y - widget.theme.border_thickness,
			pos.w + widget.theme.border_thickness * 2, pos.h + widget.theme.border_thickness * 2 },
			widget.theme.border_color);

		widget.renderer->Rectangle({ pos.x, pos.y, pos.w, pos.h },
			widget.theme.main_color);
	}

	void BeginGui(const IVec2& size) {
		if (loop_count < once_per_function_call) {
			positions.push_back({ widget_counter * start_position_info, 0, start_position_info, start_position_info });
			draggers.push_back(DragDataObject());
		}
		if (loop_count >= once_per_function_call) {
			if (size == IVec2(-1, -1)) {
				positions[widget_counter].w = start_position_info;
				positions[widget_counter].h = start_position_info;
			}
			else
				positions[widget_counter].size = size;
		}
		SDL_Rect sdl_borders = { positions[widget_counter].x - widget.theme.border_thickness,
								positions[widget_counter].y - widget.theme.border_thickness,
								positions[widget_counter].w + widget.theme.border_thickness * 2,
								positions[widget_counter].h + widget.theme.border_thickness * 2 };
		SDL_RenderSetClipRect(widget.renderer->Renderer(), &sdl_borders);

		in_widget_position = { positions[widget_counter].x, positions[widget_counter].y };

		DrawBorderedRect(positions[widget_counter]);

		DragGuiObject();
	}

	void EndGui() {
		widget_counter++;
		SDL_RenderSetClipRect(NULL, NULL);
	}

	bool ButtonGui(const std::string& text) {
		int w, h;
		font->GetSize(&w, &h, text.c_str()); 
		in_widget_position += { widget.theme.button_x_margin, widget.theme.button_y_margin };
		widget.button->UpdatePosition(in_widget_position);
		widget.button->UpdateSize(w, h);
		font->SetPosition(in_widget_position.x, in_widget_position.y);
		font->ChangeFont(text, widget.theme.main_text_color);
		if (widget.button->Hover())
			widget.renderer->Rectangle({ in_widget_position.x - widget.theme.button_x_pad, in_widget_position.y - widget.theme.button_y_pad, w + widget.theme.button_x_pad * 2, h + widget.theme.button_y_pad * 2}, widget.theme.button_hover_color);
		else
			widget.renderer->Rectangle({ in_widget_position.x - widget.theme.button_x_pad, in_widget_position.y - widget.theme.button_y_pad, w + widget.theme.button_x_pad * 2, h + widget.theme.button_y_pad * 2}, widget.theme.button_color);

		font->Render();
		in_widget_position.y += h + widget.theme.button_y_margin;
		in_widget_position -= { widget.theme.button_x_margin, widget.theme.button_y_margin };

		return (widget.button->Click(CLICK_ID));
	}

	void BeginFrame() {
		widget_counter = 0;
		if(loop_count < once_per_function_call)
			loop_count++;
	}

	void DestroyGui() {
		font->Destroy();
	}
}
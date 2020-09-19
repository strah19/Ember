#include "EmberPch.h"
#include "Widgets.h"

namespace ember {
	static Widget widget;
	std::vector<Rect> positions;
	std::vector<DragDataObject> draggers;
	int widget_counter = 0;
	int loop_count = 0;

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
			widget.cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
			if (widget.cursor == nullptr) 
				return EmberGuiError::CursorNull;
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

		draggers[widget_counter].CalculateNewPositionAfterDrag(positions[widget_counter], widget.events, widget.button->Click(1), DragDataObject::DragMask::Both);
	}

	void BeginGui(const IVec2& size) {
		if (loop_count < 2) {
			positions.push_back({ widget_counter * 100, 0, 100, 100 });
			draggers.push_back(DragDataObject());
		}
		if (loop_count >= 2) {
			if (size == IVec2(-1, -1)) {
				positions[widget_counter].w = 100;
				positions[widget_counter].h = 100;
			}
			else
				positions[widget_counter].size = size;
		}

		widget.renderer->Rectangle({ positions[widget_counter].x - widget.theme.border_thickness, positions[widget_counter].y - widget.theme.border_thickness,
			positions[widget_counter].w + widget.theme.border_thickness * 2, positions[widget_counter].h + widget.theme.border_thickness * 2 },
			widget.theme.border_color);

		widget.renderer->Rectangle({ positions[widget_counter].x, positions[widget_counter].y, positions[widget_counter].w, positions[widget_counter].h },
			widget.theme.main_color);

		DragGuiObject();
		widget_counter++;
	}

	void BeginFrame() {
		widget_counter = 0;
		if(loop_count < 2)
			loop_count++;
	}
}
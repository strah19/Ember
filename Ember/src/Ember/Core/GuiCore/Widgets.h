#ifndef WIDGETS_H
#define WIDGETS_H

#include "Structures/Rect.h"
#include "Event/EventHandler.h"
#include "Renderer/2DRenderer.h"
#include "Button.h"

namespace ember {
	enum class WidgetThemeType {
		Light, Dark, Custom, Norm
	};

	struct DragDataObject {
		bool is_moving = false;
		int pre_x = 0, pre_y = 0;

		enum class DragMask {
			Axis_x,
			Axis_y,
			Both
		};

		bool CalculateNewPositionAfterDrag(Rect& position, std::shared_ptr<Input> events, bool clicked, DragMask mask);
	};

	struct WidgetTheme {
		Color border_color = { 80, 80, 80, 255 };
		Color main_color = { 90, 90, 90, 255 };

		int border_thickness = 2;
		int resize_button_width = 20;

		WidgetThemeType type = WidgetThemeType::Norm;
	};

	struct Widget {
		WidgetTheme theme;
		std::shared_ptr<Button> button;

		std::shared_ptr<Renderer2D> renderer;
		std::shared_ptr<Input> events;

		SDL_Cursor* cursor = nullptr;

		Widget()  {
						
		}
	};

	enum class EmberGuiError {
		RendererEventsNull, CursorNull, None
	};

	EmberGuiError InitializeEmberGui(std::shared_ptr<Renderer2D> renderer, std::shared_ptr<Input> events);
	void SetTheme(WidgetTheme& theme);

	void BeginGui(const IVec2& size = { -1, -1 });
	void BeginFrame();
	void DragGuiObject();
}

#endif // !WIDGETS_H




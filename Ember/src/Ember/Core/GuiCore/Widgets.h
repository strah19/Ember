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
		Color button_hover_color = { 120, 120, 120, 255 };
		Color button_color = { 100, 100, 100, 255 };
		int button_x_margin = 6, button_y_margin = 12;
		int button_x_pad = 3, button_y_pad = 3;


		int border_thickness = 2;
		int resize_button_width = 20;
		int font_size = 13;
		Color main_text_color = { 200, 200, 200, 255 };

		WidgetThemeType type = WidgetThemeType::Norm;
	};

	struct Widget {
		WidgetTheme theme;
		std::shared_ptr<Button> button;

		std::shared_ptr<Renderer2D> renderer;
		std::shared_ptr<Input> events;

		Widget()  {
						
		}
	};

	enum class EmberGuiError {
		RendererEventsNull, None
	};

	EmberGuiError InitializeEmberGui(std::shared_ptr<Renderer2D> renderer, std::shared_ptr<Input> events);
	void SetTheme(WidgetTheme& theme);

	void BeginGui(const IVec2& size = { -1, -1 });
	void EndGui();

	void BeginFrame();
	bool ButtonGui(const std::string& text);
	void DragGuiObject();
	void DestroyGui();
	void DrawBorderedRect(const Rect& pos);
}

#endif // !WIDGETS_H




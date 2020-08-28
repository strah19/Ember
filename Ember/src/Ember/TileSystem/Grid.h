#ifndef GRID_H
#define GRID_H

#include "../Renderer/2DRenderer.h"
#include "../Core/EventHandlers/Input.h"
#include "../Structures/Vec2.h"
#include "../Core/GuiCore/Button.h"
#include "../Structures/Color.h"

namespace ember {
	struct GridComponents {
		int rows, cols;
		int block_width, block_height;
		int start_x, start_y;
	};

	class Grid {
	public:
		Grid(Renderer2D* renderer, Input* input, GridComponents& grid);
		virtual void ResizeGrid(GridComponents& grid);
		void RenderRect(int col, int row, const Color& color);
		void RenderBorder(const Color& color);
		IVec2 Hover();
		IVec2 Click(int id);
		IVec2 Hold(int id);

		void Personalize(const std::function<void(int x, int y, int bw, int bh)>& function);

		IVec2 ConvertPoints(int col, int row);
	protected:
		GridComponents grid;
		int x, y;

		Renderer2D* renderer;
		Input* input;
		Button button;

		Renderer2D* Render() { return renderer; }
		Input* Inputs() { return input; }
	};
}

#endif // !GRID_H

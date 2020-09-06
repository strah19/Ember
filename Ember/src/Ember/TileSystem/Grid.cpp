#include "EmberPch.h"
#include "Grid.h"

namespace ember {
	Grid::Grid(Renderer2D* renderer, Input* input, GridComponents& grid)
		: renderer(renderer), input(input), grid(grid), button(input, 0, 0, grid.block_width, grid.block_height), x(grid.start_x), y(grid.start_y) {
	}

	void Grid::ResizeGrid(GridComponents& grid_component) {
		grid.block_width = grid_component.block_width;
		grid.block_height = grid_component.block_height;

		grid.cols = grid_component.cols;
		grid.rows = grid_component.rows;

		grid.start_x = grid_component.start_x;
		grid.start_y = grid_component.start_y;
	}

	void Grid::RenderRect(int col, int row, const Color& color) {
		renderer->Rectangle({ grid.start_x + col * grid.block_width, grid.start_y + row * grid.block_height, grid.block_width, grid.block_height }, color);
	}

	void Grid::RenderBorder(const Color& color) {
		x = grid.start_x;
		y = grid.start_y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				renderer->Line({ x , y }, { x + grid.block_width, y }, color);
				renderer->Line({ x, y }, { x, y + grid.block_height }, color);

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y;
		}
		renderer->Line({ grid.start_x, grid.start_y + (grid.block_height * grid.rows) }, { grid.start_x + (grid.block_width * grid.cols),
						 grid.start_y + (grid.block_height * grid.rows) }, color);
		renderer->Line({ grid.start_x + (grid.block_width * grid.cols), grid.start_y }, { grid.start_x + (grid.block_width * grid.cols), 
						 grid.start_y + (grid.block_height * grid.rows) }, color);
	}

	IVec2 Grid::Hover() {
		x = grid.start_x;
		y = grid.start_y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hover()) {
					return { i, j };
				}

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Click(int id) {
		x = grid.start_x;
		y = grid.start_y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Click(id)) {
					return { i, j };
				}
				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Hold(int id) {
		x = grid.start_x;
		y = grid.start_y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hold(id)) {
					return { i, j };
				}
				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::ConvertPoints(int col, int row) {
		return { grid.start_x + (col * grid.block_width), grid.start_y + (row * grid.block_height) };
	}


	void Grid::Personalize(const std::function<void(int x, int y, int bw, int bh)>& function) {
		x = grid.start_x;
		y = grid.start_y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				function(x, y, grid.block_width, grid.block_height);

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y;
		}
	}
}
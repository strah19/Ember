#include "Grid.h"

namespace Ember {
	Grid::Grid(rRenderer* renderer, Events* events, const GridComponents& grid)
		: renderer(renderer), events(events), grid(grid), button(events, { 0, 0, grid.block_width, grid.block_height }), x(grid.start_x), y(grid.start_y), camera_position(0, 0) {
	}

	Grid::Grid() : renderer(nullptr), events(nullptr), x(0), y(0), camera_position(0, 0) {
	}

	void Grid::Initialize(rRenderer* renderer, Events* events, const GridComponents& grid) {
		this->renderer = renderer;
		this->events = events;

		this->grid = grid;
		button.Initialize(events, { 0, 0, grid.block_width, grid.block_height });
		x = grid.start_x;
		y = grid.start_y;
		camera_position = { 0, 0 };
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
		if(renderer != nullptr)
			renderer->Rectangle({ grid.start_x + col * grid.block_width - camera_position.x, grid.start_y + row * grid.block_height - camera_position.y, grid.block_width, grid.block_height }, color);
	}

	void Grid::RenderBorder(const Color& color) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		if (renderer != nullptr) {
			for (int i = 0; i < grid.cols; i++) {
				for (int j = 0; j < grid.rows; j++) {
					renderer->Line({ x , y }, { x + grid.block_width, y }, color);
					renderer->Line({ x, y }, { x, y + grid.block_height }, color);

					y += grid.block_height;
				}
				x += grid.block_width;
				y = grid.start_y - camera_position.y;
			}
			renderer->Line({ grid.start_x - camera_position.x, grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, { grid.start_x + (grid.block_width * grid.cols) - camera_position.x,
							 grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, color);
			renderer->Line({ grid.start_x + (grid.block_width * grid.cols) - camera_position.x, grid.start_y - camera_position.y }, { grid.start_x + (grid.block_width * grid.cols) - camera_position.x,
							 grid.start_y + (grid.block_height * grid.rows) - camera_position.y }, color);

		}
	}

	IVec2 Grid::Hover() {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hover())
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Click(ButtonIds id) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Click(id))
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::Hold(ButtonIds id) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				button.UpdatePosition(x, y);
				if (button.Hold(id))
					return { i, j };

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
		return { -1, -1 };
	}

	IVec2 Grid::ConvertPoints(int col, int row) {
		return { grid.start_x + (col * grid.block_width) - camera_position.x, grid.start_y + (row * grid.block_height) - camera_position.y };
	}

	void Grid::Personalize(const std::function<void(int x, int y, int bw, int bh)>& function) {
		x = grid.start_x - camera_position.x;
		y = grid.start_y - camera_position.y;

		for (int i = 0; i < grid.cols; i++) {
			for (int j = 0; j < grid.rows; j++) {
				function(x, y, grid.block_width, grid.block_height);

				y += grid.block_height;
			}
			x += grid.block_width;
			y = grid.start_y - camera_position.y;
		}
	}
}
#ifndef GRID_H
#define GRID_H

#include "Ember.h"
#include "Renderer.h"
#include "EventHandler.h"
#include "Button.h"

namespace Ember {
	struct GridComponents {
		int rows, cols;
		int block_width, block_height;
		int start_x, start_y;
	};

	class Grid {
	public:
		Grid(rRenderer* renderer, Events* events, const GridComponents& grid);
		Grid();

		void Initialize(rRenderer* renderer, Events* events, const GridComponents& grid);
		virtual void ResizeGrid(GridComponents& grid);
		void RenderRect(int col, int row, const Color& color);
		void RenderBorder(const Color& color);
		IVec2 Hover();
		IVec2 Click(ButtonIds id);
		IVec2 Hold(ButtonIds id);

		void Personalize(const std::function<void(int x, int y, int bw, int bh)>& function);

		IVec2 ConvertPoints(int col, int row);
		inline GridComponents GridComponent() const { return grid; }

		Events* Event() { return events; }
		rRenderer* Renderer() { return renderer; }

		void SetCameraPosition(const IVec2& camera_position) { this->camera_position = camera_position; }
	protected:
		GridComponents grid;
		int x, y;

		IVec2 camera_position;

		rRenderer* renderer;
		Events* events;
		Button button;
	};

	template <class T>
	class DataGrid : public Grid {
	public:
		void InitializeData() {
			data = new int* [grid.rows];
			for (int i = 0; i < grid.rows; ++i) {
				data[i] = new int[grid.cols];
			}
		}

		~DataGrid() {
			if (data) {
				for (int i = 0; i < grid.rows; ++i) {
					delete[] data[i];
				}
				delete[] data;
			}
		}

		void SetData(int col, int row, const T& data) {
			this->data[row][col] = data;
		}

		T& GetData(int col, int row) {
			return data[row][col];
		}
	private:
		T** data = nullptr;
	};
}

#endif // !GRID_H

#include "FunctionIt.h"
#include "Shape.h"
#include <sstream>

namespace Ember
{
	void Button::create_button(SDL_Rect btn)
	{
		Position = btn;
	}

	void Button::create_button(int x, int y, int w, int h)
	{
		Position = { x, y, w, h };
	}

	bool Button::hover()
	{
		if (mouseEvent.x >= Position.x && mouseEvent.x <= Position.x + Position.w && mouseEvent.y >= Position.y && mouseEvent.y <= Position.y + Position.h) {
			return true;
		}
		else
			return false;
		return true;
	}

	bool Button::click(int ID)
	{
		if (mouseEvent.Down && mouseEvent.Id == ID) {
			if (mouseEvent.x >= Position.x && mouseEvent.x <= Position.x + Position.w && mouseEvent.y >= Position.y && mouseEvent.y <= Position.y + Position.h) {
				mouseEvent.Down = false;
				return true;
			}
		}
		return false;
	}

	int Button::in_out_click(int ID)
	{
		if (mouseEvent.Down && mouseEvent.Id == ID) {
			if (Clicked == false && mouseEvent.x >= Position.x && mouseEvent.x <= Position.x + Position.w && mouseEvent.y >= Position.y && mouseEvent.y <= Position.y + Position.h) {
				Clicked = true;
				mouseEvent.Down = false;
				InOut = 2;
			}
			else if (Clicked == true && mouseEvent.x >= Position.x && mouseEvent.x <= Position.x + Position.w && mouseEvent.y >= Position.y && mouseEvent.y <= Position.y + Position.h) {
				Clicked = false;
				mouseEvent.Down = false;
				InOut = 1;
			}
		}
		return InOut;
	}

	void Button::update_position(int x, int y)
	{
		Position.x = x;
		Position.y = y;
	}

	void Button::update_size(int w, int h)
	{
		Position.w = w;
		Position.h = h;
	}

	bool Button::hold(int ID)
	{
		if (mouseEvent.Down && mouseEvent.Id == ID) {
			if (mouseEvent.x >= Position.x && mouseEvent.x <= Position.x + Position.w && mouseEvent.y >= Position.y && mouseEvent.y <= Position.y + Position.h) {
				return true;
			}
		}
		return false;
	}

	Grid::Grid(int x, int y, int row, int col, int widthSize, int heightSize)
	{
		Rows = row;
		Cols = col;
		BlockHeight = heightSize;
		BlockWidth = widthSize;

		StartX = x;
		StartY = y;

		X = x;
		Y = y;
		GridButtons.create_button(X, Y, BlockWidth, BlockHeight);
	}

	void Grid::render_rects(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b)
	{
		X = StartX;
		Y = StartY;
		int l = -1;
		for (unsigned int i = 0; i < Rows; i++) {
			for (unsigned int j = 0; j < Cols; j++) {
				l++;

				rectangle_fill(renderer, X + 1, Y + 1, BlockWidth - 1, BlockHeight - 1, r, g, b);

				X += BlockWidth;
				if (l == Rows - 1) {
					Y += BlockHeight;
					X = StartX;
					l = -1;
				}
			}
		}
	}

	void Grid::render_border(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b)
	{
		X = StartX;
		Y = StartY;
		int l = -1;
		for (unsigned int i = 0; i < Rows; i++) {
			for (unsigned int j = 0; j < Cols; j++) {
				l++;

				horizontal_line(renderer, X, X + BlockWidth, Y, r, g, b);
				vertical_line(renderer, Y, Y + BlockHeight, X, r, g, b);

				X += BlockWidth;
				if (l == Rows - 1) {
					Y += BlockHeight;
					X = StartX;
					l = -1;
				}
			}
		}
		horizontal_line(renderer, X, X + (BlockWidth * Rows), Y, r, g, b);
		vertical_line(renderer, StartY, StartY + (BlockHeight * Cols), X + (BlockWidth * Rows), r, g, b);
	}

	std::pair<int, int> Grid::hover()
	{
		X = StartX;
		Y = StartY;
		GridButtons.update_position(X, Y);
		int l = -1;
		for (unsigned int i = 0; i < Rows; i++) {
			for (unsigned int j = 0; j < Cols; j++) {
				l++;

				if (GridButtons.hover()) {
					return std::make_pair(i, j);
				}

				X += BlockWidth;
				if (l == Rows - 1) {
					Y += BlockHeight;
					X = StartX;
					l = -1;
				}
				GridButtons.update_position(X, Y);
			}
		}
		return std::make_pair(-1, -1);
	}

	std::pair<int, int> Grid::click(int ID)
	{
		X = StartX;
		Y = StartY;
		GridButtons.update_position(X, Y);
		int l = -1;
		for (unsigned int i = 0; i < Rows; i++) {
			for (unsigned int j = 0; j < Cols; j++) {
				l++;

				if (GridButtons.click(ID)) {
					return std::make_pair(i, j);
				}

				X += BlockWidth;
				if (l == Rows - 1) {
					Y += BlockHeight;
					X = StartX;
					l = -1;
				}
				GridButtons.update_position(X, Y);
			}
		}
		return std::make_pair(-1, -1);
	}

	int StartPointX, StartpointY;
	int PermaX[100], PermaY[100];
	int PermaW[100], PermaH[100];
	int Counter = 0;

	bool ReadyForEndless = false;

	void endless_rects(SDL_Renderer* renderer, int ID, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		if (mouseEvent.Down && mouseEvent.Id == ID && !ReadyForEndless) {
			StartPointX = mouseEvent.x;
			StartpointY = mouseEvent.y;
			ReadyForEndless = true;
		}
		if (ReadyForEndless) {
			rectangle_fill(renderer, StartPointX, StartpointY, mouseEvent.x - StartPointX, mouseEvent.y - StartpointY, r, g, b, a);
			if (mouseEvent.Up) {
				PermaX[Counter] = StartPointX;
				PermaY[Counter] = StartpointY;
				PermaW[Counter] = mouseEvent.x - StartPointX;
				PermaH[Counter] = mouseEvent.y - StartpointY;
				Counter++;
			}
		}
		if (mouseEvent.Up) {
			ReadyForEndless = false;
		}
		if (mouseEvent.Down && mouseEvent.Id == ID + 1) {
			for (int i = 0; i < Counter; i++) {
				PermaX[i] = PermaY[i] = PermaW[i] = PermaH[i] = 0;
			}
			Counter = 0;
		}

		for (int i = 0; i < Counter; i++) {
			rectangle_fill(renderer, PermaX[i], PermaY[i], PermaW[i], PermaH[i], r, g, b, a);
		}
	}
	void save_rects()
	{
		File SaveRects("Rects.txt", true);
	}
}
#ifndef EMBER_FUNCTIONAL_H
#define EMBER_FUNCTIONAL_H

#include "Events.h"
#include "FileManager.h"

namespace Ember
{
	enum class CursorType
	{
		Ibeam = SDL_SYSTEM_CURSOR_IBEAM, wait = SDL_SYSTEM_CURSOR_WAIT, crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
		smallCursor = SDL_SYSTEM_CURSOR_WAITARROW, hand = SDL_SYSTEM_CURSOR_HAND, slashedCircle = SDL_SYSTEM_CURSOR_NO,
		fourPointedArrows = SDL_SYSTEM_CURSOR_SIZEALL, pointingNorthSouth = SDL_SYSTEM_CURSOR_SIZENS, pointingWestEast = SDL_SYSTEM_CURSOR_SIZEWE,
		pointingNorthWestToSouthEast = SDL_SYSTEM_CURSOR_SIZENWSE, pointingNorthEastToSouthWest = SDL_SYSTEM_CURSOR_SIZENESW, arrow = SDL_SYSTEM_CURSOR_ARROW
	};

	class Cursor
	{
	
	};

	class Button
	{
	public:
		void create_button(SDL_Rect btn);
		void create_button(int x, int y, int w, int h);
		bool hover();
		bool click(int ID);
		void update_position(int x, int y);
		void update_size(int w, int h);
		int in_out_click(int ID);
		bool hold(int ID);
	private:
		SDL_Rect Position = { 0, 0, 0, 0 };
		int InOut = 1;
		bool Clicked = false;
	};

	class Grid
	{
	public:
		Grid(int x, int y, int row, int col, int widthSize, int heightSize);
		void render_rects(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
		void render_border(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
		std::pair<int, int> hover();
		std::pair<int, int> click(int ID);
	private:
		unsigned int Rows = 0, Cols = 0;
		int BlockWidth = 0, BlockHeight = 0;
		int X = 0, Y = 0;
		int StartX = 0, StartY = 0;
		Button GridButtons;
	};

	void endless_rects(SDL_Renderer* renderer, int ID, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
	void save_rects();
}

#endif // !EMBER_FUNCTIONAL_H
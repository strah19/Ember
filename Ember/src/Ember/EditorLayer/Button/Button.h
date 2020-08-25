#ifndef BUTTON_H
#define BUTTON_H

#include "../../Renderer/2DRenderer.h"
#include "../../Structures/Rect.h"
#include "../../Core/EventHandler.h"

namespace ember {
	class Button {
	public:
		Button(EventHandler* events, Rect& button);
		Button(EventHandler* events, int x, int y, int w, int h);
		bool Hover();
		bool Click(int id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);
		bool Hold(int id);
		bool InOut(int id);
		inline Rect Position() { return position; }
	protected:
		Rect position;
		EventHandler* events;
		bool in;
		bool clicked;
	};
}

#endif // !BUTTON_H

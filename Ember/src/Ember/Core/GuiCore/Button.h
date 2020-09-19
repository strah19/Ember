#ifndef BUTTON_H
#define BUTTON_H

#include "Renderer/2DRenderer.h"
#include "Structures/Rect.h"
#include "Event/EventHandler.h"

namespace ember {
	class Button {
	public:
		Button(std::shared_ptr<Input> events, Rect& button);
		Button(std::shared_ptr<Input> events, int x, int y, int w, int h);
		bool Hover();
		bool Click(int id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);

		void UpdatePosition(IVec2& pos) { position.pos = pos; }
		void UpdateSize(IVec2 size) { position.size = size; }
		
		bool Hold(int id);
		bool InOut(int id);
		inline Rect Position() { return position; }
	protected:
		Rect position;
		std::shared_ptr<Input> events;
		bool in;
		bool clicked;
	};
}

#endif // !BUTTON_H

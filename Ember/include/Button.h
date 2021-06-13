#ifndef BUTTON_H
#define BUTTON_H

#include "EventHandler.h"
#include "Ember.h"

namespace Ember {
	class Button {
	public:
		Button(Events* events, const Rect& button);
		Button();

		void Initialize(Events* events, const Rect& button);
		bool Hover();
		bool Click(ButtonIds id);
		void UpdatePosition(int x, int y);
		void UpdateSize(int w, int h);

		void UpdatePosition(IVec2& pos) { position.pos = pos; }
		void UpdateSize(IVec2 size) { position.size = size; }
		 
		bool Hold(ButtonIds id);
		bool InOut(ButtonIds id);
		inline Rect Position() { return position; }
	private:
		Rect position;
		Events* events;
		bool in;
		bool clicked;
	};
}

#endif // !BUTTON_H

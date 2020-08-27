#ifndef EDITOR_H
#define EDITOR_H

#include "Structures/Vec2.h"
#include "Structures/Rect.h"

namespace ember {
	class ScaleableObject {
	public:
		ScaleableObject(const Rect& position, int* viewport_width, int* viewport_height);
		ScaleableObject();

		Rect UpdatePosition();
	private:
		void CalculatePercentages();

		Rect start_position;
		FRect percantages;
		int* width;
		int* height;
	};
}

#endif // !EDITOR_H

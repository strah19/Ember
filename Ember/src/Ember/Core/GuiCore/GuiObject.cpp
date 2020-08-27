#include "EmberPch.h"
#include "GuiObjects.h"

namespace ember {
	ScaleableObject::ScaleableObject(const Rect& position, int* viewport_width, int* viewport_height)
		: start_position(position), percantages({ 0, 0, 0, 0 }), width(viewport_width), height(viewport_height) {
		CalculatePercentages();
	}

	ScaleableObject::ScaleableObject() 
		: start_position({ 0, 0, 0, 0 }), percantages({ 0, 0, 0, 0 }), width(nullptr), height(nullptr) {
		CalculatePercentages();
	}

	Rect ScaleableObject::UpdatePosition()
	{
		return { (int) percantages.x * (*width / 100), (int) percantages.y * (*height / 100), (int) percantages.w * (*width / 100), (int) percantages.h * (*height / 100) };
	}

	void ScaleableObject::CalculatePercentages() {
		if (width != nullptr && height != nullptr) {
			percantages.x = ((float)start_position.x / (float)*width) * 100;
			percantages.y = ((float)start_position.y / (float)*height) * 100;
			percantages.w = ((float)start_position.w / (float)*width) * 100;
			percantages.h = ((float)start_position.h / (float)*height) * 100;
		}
	}
}

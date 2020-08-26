#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "../Entity.h"

namespace ember {
	class PositionComponent : public Component {
	public:
		PositionComponent(float x, float y, float w, float h);
		PositionComponent(const FRect& position);
		virtual ~PositionComponent();

		void Init();
		void Update();
		void Render();

		void ChangePosition(float x, float y);
		void ChangeSize(float w, float h);
		void UpdatePosition(float dx, float dy);
		void UpdateSize(float dw, float dh);
		void Angle(float angle);

		inline FRect Position() { return position; }
		inline float GetAngle() { return angle; }

		void Scale(int on_x, int on_y);
	private:
		FRect position;
		float angle;
	};
}

#endif // !POSITION_COMPONENT_H

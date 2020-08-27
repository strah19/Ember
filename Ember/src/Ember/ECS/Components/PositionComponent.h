#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "../Entity.h"

namespace ember {
	class PositionComponent : public Component {
	public:
		PositionComponent(int x, int y, int w, int h);
		PositionComponent(const Rect& position);
		virtual ~PositionComponent();

		void Init();
		void Update();
		void Render();

		void ChangePosition(int x, int y);
		void ChangeSize(int w, int h);
		void UpdatePosition(int dx, int dy);
		void UpdateSize(int dw, int dh);
		void Angle(float angle);

		inline Rect Position() { return position; }
		inline float GetAngle() { return angle; }

		void Scale(int on_x, int on_y);
	private:
		Rect position;
		float angle;
	};
}

#endif // !POSITION_COMPONENT_H

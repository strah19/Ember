#ifndef COLLISION_H
#define COLLISION_H

#include "../Structures/Rect.h"
#include "../Structures/Circle.h"

namespace ember {
	bool PointVsRect(const Vec2& point, const FRect& rect);
	bool RectVsRect(const FRect& rect1, const FRect& rect2);

	bool PointVsRect(const IVec2& point, const Rect& rect);
	bool RectVsRect(const Rect& rect1, const Rect& rect2);

	bool RayVsRect(const Vec2& ray, const Vec2& ray_dir, const FRect& rect, Vec2& contact, Vec2& contact_normal, float& hit_near);
	bool DynamicRectVsRect(const FRect& rect1, const FRect& rect2, Vec2& contact, Vec2& contact_normal, float& hit_near, Vec2& velocity);
	bool PointToCircle(Vec2& point, Circle& circle);
	bool PointToCircle(Vec2& point, CircleF& circle);
}

#endif // !COLLISION_H

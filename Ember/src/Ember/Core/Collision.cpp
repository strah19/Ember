#include "EmberPch.h"
#include "Collision.h"

namespace ember {
	bool PointVsRect(const Vec2& point, const FRect& rect) {
		return (point.x >= rect.pos.x && point.y >= rect.pos.y && point.x < rect.pos.x + rect.w && point.y < rect.pos.y + rect.h);
	}

	bool RectVsRect(const FRect& rect1, const FRect& rect2) {
		return (rect1.pos.x < rect2.pos.x + rect2.w && rect1.pos.x + rect1.w > rect2.pos.x &&
			rect1.pos.y < rect2.pos.y + rect2.h && rect1.pos.y + rect1.h > rect2.pos.y);
	}

	bool PointVsRect(const IVec2& point, const Rect& rect) {
		return (point.x >= rect.pos.x && point.y >= rect.pos.y && point.x < rect.pos.x + rect.w && point.y < rect.pos.y + rect.h);
	}

	bool RectVsRect(const Rect& rect1, const Rect& rect2) {
		return (rect1.pos.x < rect2.pos.x + rect2.w && rect1.pos.x + rect1.w > rect2.pos.x &&
			rect1.pos.y < rect2.pos.y + rect2.h && rect1.pos.y + rect1.h > rect2.pos.y);
	}

	bool RayVsRect(const Vec2& ray, const Vec2& ray_dir, const FRect& rect, Vec2& contact, Vec2& contact_normal, float& hit_near) {
		Vec2 near = (rect.pos - ray) / ray_dir;
		Vec2 far = (rect.pos + rect.size - ray) / ray_dir;

		if (near.x > far.x)
			std::swap(near.x, far.x);
		if (near.y > far.y)
			std::swap(near.y, far.y);

		if (near.x > far.y || near.y > far.x)
			return false;

		hit_near = std::max(near.x, near.y);
		float hitFar = std::min(far.x, far.y);

		if (hitFar < 0)
			return false;

		contact.x = ray.x + hit_near * ray_dir.x;
		contact.y = ray.y + hit_near * ray_dir.y;

		if (near.x > near.y)
		{
			if (ray_dir.x < 0)
			{
				contact_normal = { 1, 0 };
			}
			else
			{
				contact_normal = { -1, 0 };
			}
		}
		else if (near.x < near.y)
		{
			if (ray_dir.y < 0)
			{
				contact_normal = { 0, 1 };
			}
			else
			{
				contact_normal = { 0, -1 };
			}
		}

		return true;
	}

	bool DynamicRectVsRect(const FRect& rect1, const FRect& rect2, Vec2& contact, Vec2& contact_normal, float& hit_near, Vec2& velocity) {
		if (velocity.x == 0 && velocity.y == 0)
			return false;

		FRect expand = { 0, 0, 0, 0 };
		expand.pos = rect2.pos - rect1.size / 2 + (rect1.size / 2 + 1);
		expand.size = rect2.size + rect1.size - 2;

		if (RayVsRect(rect1.pos + rect1.size, velocity, expand, contact, contact_normal, hit_near))
		{
			if (hit_near <= 1.0f)
			{
				return true;
			}
		}

		return false;
	}


	bool PointToCircle(Vec2& point, Circle& circle) {
		int dx = (int)abs((int)point.x - circle.x);
		int dy = (int)abs((int)point.y - circle.y);

		if (dx > circle.radius || dy > circle.radius) {
			return false;
		}

		if (dx + dy <= circle.radius) {
			return true;
		}

		if (dx * dx + dy * dy <= circle.radius * circle.radius) {
			return true;
		}
		else {
			return false;
		}
	}

	bool PointToCircle(Vec2& point, CircleF& circle) {
		float dx = (float)abs(point.x - circle.x);
		float dy = (float)abs(point.y - circle.y);

		if (dx > circle.radius || dy > circle.radius) {
			return false;
		}

		if (dx + dy <= circle.radius) {
			return true;
		}

		if (dx * dx + dy * dy <= circle.radius * circle.radius) {
			return true;
		}
		else {
			return false;
		}
	}
}
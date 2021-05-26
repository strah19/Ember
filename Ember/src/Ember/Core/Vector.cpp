#include "Vector.h"

namespace Ember {
	Vec2::Vec2(const float x, const float y)
		:x(x), y(y) {
	}

	std::ostream& operator<<(std::ostream& os, const Vec2& vec2) {
		os << vec2.x << " " << vec2.y;
		return os;
	}

	Vec2 operator+(const Vec2& v1, const Vec2& v2) {
		return (Vec2(v1.x + v2.x, v1.y + v2.y));
	}

	Vec2 operator-(const Vec2& v1, const Vec2& v2) {
		return (Vec2(v1.x - v2.x, v1.y - v2.y));
	}

	Vec2 operator*(const Vec2& v1, const Vec2& v2) {
		return (Vec2(v1.x * v2.x, v1.y * v2.y));
	}

	Vec2 operator/(const Vec2& v1, const Vec2& v2) {
		return (Vec2(v1.x / v2.x, v1.y / v2.y));
	}

	Vec2 operator+(const Vec2& v1, const float s) {
		return (Vec2(v1.x + s, v1.y + s));
	}

	Vec2 operator-(const Vec2& v1, const float s) {
		return (Vec2(v1.x - s, v1.y - s));
	}

	Vec2 operator*(const Vec2& v1, const float s) {
		return (Vec2(v1.x * s, v1.y * s));
	}

	Vec2 operator/(const Vec2& v1, const float s) {
		return (Vec2(v1.x / s, v1.y / s));
	}

	void Vec2::operator+=(const float scalar) {
		this->x += scalar;
		this->y += scalar;
	}

	void Vec2::operator-=(const float scalar) {
		this->x -= scalar;
		this->y -= scalar;
	}

	void Vec2::operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	void Vec2::operator/=(const float scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void Vec2::operator+=(const Vec2& vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void Vec2::operator-=(const Vec2& vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void Vec2::operator*=(const Vec2& vec) {
		this->x *= vec.x;
		this->y *= vec.y;
	}

	void Vec2::operator/=(const Vec2& vec) {
		this->x /= vec.x;
		this->y /= vec.y;
	}

	void Vec2::Negate() {
		this->x *= -1;
		this->y *= -1;
	}

	float Vec2::Magnitude() {
		return (std::sqrt((this->x * this->x) + (this->y * this->y)));
	}

	Vec2 Vec2::Normalize() {
		Vec2 return_vec2 = { this->x, this->y };
		float m = Magnitude();
		return_vec2.x /= m;
		return_vec2.y /= m;

		return return_vec2;
	}

	bool operator!=(const Vec2& v1, const Vec2& v2) {
		if (v1.x != v2.x && v1.y != v2.y) {
			return true;
		}
		return false;
	}

	bool operator==(const Vec2& v1, const Vec2& v2) {
		if (v1.x == v2.x && v1.y == v2.y) {
			return true;
		}
		return false;
	}

	float Vec2::DotProduct(Vec2& vec2) {
		return ((this->x * vec2.x) + (this->y * vec2.y));
	}

	IVec2::IVec2(const int x, const int y)
		:x(x), y(y) {
	}

	std::ostream& operator<<(std::ostream& os, const IVec2& vec2) {
		os << vec2.x << " " << vec2.y;
		return os;
	}

	IVec2 operator+(const IVec2& v1, const IVec2& v2) {
		return (IVec2(v1.x + v2.x, v1.y + v2.y));
	}

	IVec2 operator-(const IVec2& v1, const IVec2& v2) {
		return (IVec2(v1.x - v2.x, v1.y - v2.y));
	}

	IVec2 operator*(const IVec2& v1, const IVec2& v2) {
		return (IVec2(v1.x * v2.x, v1.y * v2.y));
	}

	IVec2 operator/(const IVec2& v1, const IVec2& v2) {
		return (IVec2(v1.x / v2.x, v1.y / v2.y));
	}

	IVec2 operator+(const IVec2& v1, const int s) {
		return (IVec2(v1.x + s, v1.y + s));
	}

	IVec2 operator-(const IVec2& v1, const int s) {
		return (IVec2(v1.x - s, v1.y - s));
	}

	IVec2 operator*(const IVec2& v1, const int s) {
		return (IVec2(v1.x * s, v1.y * s));
	}

	IVec2 operator/(const IVec2& v1, const int s) {
		return (IVec2(v1.x / s, v1.y / s));
	}

	void IVec2::operator+=(const int scalar) {
		this->x += scalar;
		this->y += scalar;
	}

	void IVec2::operator-=(const int scalar) {
		this->x -= scalar;
		this->y -= scalar;
	}

	void IVec2::operator*=(const int scalar) {
		this->x *= scalar;
		this->y *= scalar;
	}

	void IVec2::operator/=(const int scalar) {
		this->x /= scalar;
		this->y /= scalar;
	}

	void IVec2::operator+=(const IVec2& vec) {
		this->x += vec.x;
		this->y += vec.y;
	}

	void IVec2::operator-=(const IVec2& vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	void IVec2::operator*=(const IVec2& vec) {
		this->x *= vec.x;
		this->y *= vec.y;
	}

	void IVec2::operator/=(const IVec2& vec) {
		this->x /= vec.x;
		this->y /= vec.y;
	}

	void IVec2::Negate() {
		this->x *= -1;
		this->y *= -1;
	}

	int IVec2::Magnitude() {
		return ((int)std::sqrt((this->x * this->x) + (this->y * this->y)));
	}

	IVec2 IVec2::Normalize() {
		IVec2 return_vec2 = { this->x, this->y };
		int m = Magnitude();
		return_vec2.x /= m;
		return_vec2.y /= m;

		return return_vec2;
	}

	bool operator!=(const IVec2& v1, const IVec2& v2) {
		if (v1.x != v2.x && v1.y != v2.y) {
			return true;
		}
		return false;
	}

	bool operator==(const IVec2& v1, const IVec2& v2) {
		if (v1.x == v2.x && v1.y == v2.y) {
			return true;
		}
		return false;
	}

	int IVec2::DotProduct(IVec2& vec2) {
		return ((this->x * vec2.x) + (this->y * vec2.y));
	}
}
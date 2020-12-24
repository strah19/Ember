#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

namespace Ember {
	class Vec2 {
	public:
		float x;
		float y;

		Vec2(const float x, const float y);
		Vec2() :x(0), y(0) {}

		friend std::ostream& operator<<(std::ostream& os, const Vec2& vec2);

		friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
		friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
		friend Vec2 operator*(const Vec2& v1, const Vec2& v2);
		friend Vec2 operator/(const Vec2& v1, const Vec2& v2);

		friend Vec2 operator+(const Vec2& v1, const float s);
		friend Vec2 operator-(const Vec2& v1, const float s);
		friend Vec2 operator*(const Vec2& v1, const float s);
		friend Vec2 operator/(const Vec2& v1, const float s);

		void operator+=(const float scalar);
		void operator-=(const float scalar);
		void operator*=(const float scalar);
		void operator/=(const float scalar);

		void operator+=(const Vec2& vec);
		void operator-=(const Vec2& vec);
		void operator*=(const Vec2& vec);
		void operator/=(const Vec2& vec);

		friend bool operator!=(const Vec2& v1, const Vec2& v2);
		friend bool operator==(const Vec2& v1, const Vec2& v2);

		void Negate();
		float Magnitude();
		Vec2 Normalize();
		float DotProduct(Vec2& vec2);
	};

	Vec2 operator+(const Vec2& v1, const Vec2& v2);
	Vec2 operator-(const Vec2& v1, const Vec2& v2);
	Vec2 operator*(const Vec2& v1, const Vec2& v2);
	Vec2 operator/(const Vec2& v1, const Vec2& v2);

	Vec2 operator+(const Vec2& v1, const float s);
	Vec2 operator-(const Vec2& v1, const float s);
	Vec2 operator*(const Vec2& v1, const float s);
	Vec2 operator/(const Vec2& v1, const float s);

	bool operator!=(const Vec2& v1, const Vec2& v2);
	bool operator==(const Vec2& v1, const Vec2& v2);

	class IVec2 {
	public:
		int x;
		int y;

		IVec2(const int x, const int y);
		IVec2() :x(0), y(0) {}

		friend std::ostream& operator<<(std::ostream& os, const IVec2& vec2);

		friend IVec2 operator+(const IVec2& v1, const IVec2& v2);
		friend IVec2 operator-(const IVec2& v1, const IVec2& v2);
		friend IVec2 operator*(const IVec2& v1, const IVec2& v2);
		friend IVec2 operator/(const IVec2& v1, const IVec2& v2);

		friend IVec2 operator+(const IVec2& v1, const int s);
		friend IVec2 operator-(const IVec2& v1, const int s);
		friend IVec2 operator*(const IVec2& v1, const int s);
		friend IVec2 operator/(const IVec2& v1, const int s);

		void operator+=(const int scalar);
		void operator-=(const int scalar);
		void operator*=(const int scalar);
		void operator/=(const int scalar);

		void operator+=(const IVec2& vec);
		void operator-=(const IVec2& vec);
		void operator*=(const IVec2& vec);
		void operator/=(const IVec2& vec);

		friend bool operator!=(const IVec2& v1, const IVec2& v2);
		friend bool operator==(const IVec2& v1, const IVec2& v2);

		void Negate();
		int Magnitude();
		IVec2 Normalize();
		int DotProduct(IVec2& vec2);
	};

	IVec2 operator+(const IVec2& v1, const IVec2& v2);
	IVec2 operator-(const IVec2& v1, const IVec2& v2);
	IVec2 operator*(const IVec2& v1, const IVec2& v2);
	IVec2 operator/(const IVec2& v1, const IVec2& v2);

	IVec2 operator+(const IVec2& v1, const int s);
	IVec2 operator-(const IVec2& v1, const int s);
	IVec2 operator*(const IVec2& v1, const int s);
	IVec2 operator/(const IVec2& v1, const int s);

	bool operator!=(const IVec2& v1, const IVec2& v2);
	bool operator==(const IVec2& v1, const IVec2& v2);
}

#endif // !VECTOR_H

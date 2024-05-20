#pragma once
#include <ostream>
#include <glm/vec2.hpp>

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const glm::vec2& v) : x(v.x), y(v.y) {}

	float x;
	float y;

	operator glm::vec2() const
	{
        return glm::vec2(x, y);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

// --------------------------------------------
// Vector2 with Vector2
// --------------------------------------------

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vector2 operator*(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

inline Vector2 operator/(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
}

// --------------------------------------------
// Vector2 with float
// --------------------------------------------

inline Vector2 operator*(const Vector2& lhs, float rhs)
{
	return Vector2(lhs.x * rhs, lhs.y * rhs);
}

inline Vector2 operator*(float lhs, const Vector2& rhs)
{
	return Vector2(lhs * rhs.x, lhs * rhs.y);
}

inline Vector2 operator/(const Vector2& lhs, float rhs)
{
	return Vector2(lhs.x / rhs, lhs.y / rhs);
}
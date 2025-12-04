#pragma once
#include <glm/vec3.hpp>

#include "util/common.h"
WARNING_DISABLE
#include "bullet/LinearMath/btVector3.h"
WARNING_ENABLE
#include "ostream"
#include "vector2.h"

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float v) : x(v), y(v), z(v) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(const btVector3& v) : x(v.x()), y(v.y()), z(v.z()) {}

	float x;
	float y;
	float z;


	operator glm::vec3() const
	{
        return glm::vec3(x, y, z);
    }

	operator btVector3() const
	{
		return {x,y,z};
	}

	static Vector3 Zero();

	static Vector3 One();

	static Vector3 Forward();

	static Vector3 Backward();

	static Vector3 Up();

	static Vector3 Down();

	static Vector3 Right();

	static Vector3 Left();

	static float Length(const Vector3& v);

	static Vector3 Normalize(const Vector3& v);

	static Vector3 Cross(const Vector3& a, const Vector3& b);

	static float Distance(const Vector3& a, const Vector3& b);

	static float Dot(const Vector3& a, const Vector3& b);

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float t);
};

// --------------------------------------------
// Helper functions
// --------------------------------------------

inline Vector3 Vector3::Zero()
{
	return Vector3(0, 0, 0);
}

inline Vector3 Vector3::One()
{
	return Vector3(1, 1, 1);
}

inline Vector3 Vector3::Forward()
{
	return Vector3(0, 0, 1);
}

inline Vector3 Vector3::Backward()
{
	return Vector3(0, 0, -1);
}

inline Vector3 Vector3::Up()
{
	return Vector3(0, 1, 0);
}

inline Vector3 Vector3::Down()
{
	return Vector3(0, -1, 0);
}

inline Vector3 Vector3::Right()
{
	// Flipped because of OpenGL coordinate system
	return Vector3(-1, 0, 0);
}

inline Vector3 Vector3::Left()
{
	// Flipped because of OpenGL coordinate system
	return Vector3(1, 0, 0);
}

inline float Vector3::Length(const Vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vector3 Vector3::Normalize(const Vector3& v)
{
	const float length = Length(v);
	return Vector3(v.x / length, v.y / length, v.z / length);
}

inline Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return Vector3(a.y * b.z - a.z * b.y,
	               a.z * b.x - a.x * b.z,
	               a.x * b.y - a.y * b.x);
}

inline float Vector3::Distance(const Vector3& a, const Vector3& b)
{
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
}

inline float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Vector3::Lerp(const Vector3& a, const Vector3& b, float t)
{
	return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

// --------------------------------------------
// Comparison operators
// -------------------------------------------

inline bool operator==(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

inline bool operator!=(const Vector3& lhs, const Vector3& rhs)
{
	return !(lhs == rhs);
}

// --------------------------------------------
// Vector3 with Vector3
// --------------------------------------------

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline Vector3 operator +=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline Vector3 operator*(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline Vector3 operator/(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

// --------------------------------------------
// Vector3 with float
// --------------------------------------------

inline Vector3 operator*(const Vector3& lhs, float rhs)
{
	return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

inline Vector3 operator*(float lhs, const Vector3& rhs)
{
	return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

inline Vector3 operator/(const Vector3& lhs, float rhs)
{
	return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

// --------------------------------------------
// Vector3 with Vector2
// --------------------------------------------

inline Vector3 operator+(const Vector3& lhs, const Vector2& rhs)
{
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z);
}

inline Vector3 operator+(const Vector2& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, rhs.z);
}

inline Vector3 operator-(const Vector3& lhs, const Vector2& rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z);
}

inline Vector3 operator-(const Vector2& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, rhs.z);
}

inline Vector3 operator*(const Vector3& lhs, const Vector2& rhs)
{
	return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z);
}

inline Vector3 operator*(const Vector2& lhs, const Vector3& rhs)
{
	return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, rhs.z);
}
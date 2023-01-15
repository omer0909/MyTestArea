#include <Vector2.hpp>

Vector2::Vector2() {}

Vector2::Vector2(float _x, float _y) : x(_x), y(_y) {}

Vector2::~Vector2() {}

Vector2::Vector2(const Vector2 &a) { *this = a; }

Vector2 &Vector2::operator=(const Vector2 &a)
{
	x = a.x;
	y = a.y;
	return *this;
}

std::ostream &operator<<(std::ostream &o, Vector2 const &v)
{
	o << "{" << v.x << ", " << v.y << " }" << std::endl;
	return (o);
}

Vector2 Vector2::operator-(const Vector2 &a) const
{
	return Vector2(x - a.x, y - a.y);
}

Vector2 Vector2::operator+(const Vector2 &a) const
{
	return Vector2(x + a.x, y + a.y);
}

Vector2 Vector2::operator*(const Vector2 &a) const
{
	return Vector2(x * a.x, y * a.y);
}

Vector2 Vector2::operator/(const float &a) const
{
	float factor = 1.0f / a;
	return Vector2(x * factor, y * factor);
}

Vector2 Vector2::operator*(const float &a) const
{
	return Vector2(x * a, y * a);
}

float Vector2::Magnitude() { return std::sqrt(SqrMagnitude()); }

float Vector2::SqrMagnitude() { return x * x + y * y; }

float Vector2::DotProduct(const Vector2 &a, const Vector2 &b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2 Vector2::Normalized() { return *this / Magnitude(); }
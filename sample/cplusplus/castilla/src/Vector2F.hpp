#pragma once

template <typename T>
class Vector2F
{
public:
	Vector2F<T>() :
		x(0),
		y(0)
	{}

	Vector2F<T>(T a, T b) :
		x(a),
		y(b)
	{}

	Vector2F<T> operator+(const Vector2F<T>& rhs) { return Vector2F<T>(x + rhs.x, y + rhs.y); }
	Vector2F<T> operator-(const Vector2F<T>& rhs) { return Vector2F<T>(x - rhs.x, y - rhs.y); }
	Vector2F<T>& operator+=(const Vector2F<T>& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vector2F<T>& operator-=(const Vector2F<T>& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	void operator++() 	{ ++x; ++y; }
	void operator--()	{ --x; --y; }
	void operator++(int) { x++; y++; }
	void operator--(int) { x--; y--; }
	bool operator==(const Vector2F<T>& rhs) const { return (x == rhs.x) && (y == rhs.y); }

	T x;
	T y;
};

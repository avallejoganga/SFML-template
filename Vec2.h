#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

template<typename T>
class Vec2
{
public:
	T x = 0;
	T y = 0;

	Vec2() = default;

	Vec2(T xin, T yin) :
		x(xin), y(yin)
	{
	}


	Vec2(const sf::Vector2<T>& vec) :
		x(vec.x), y(vec.y)
	{
	}

	//Permite la conversión automática al Vector2 de SFML
	operator sf::Vector2<T>()
	{
		return sf::Vector2<T>(x, y);
	}

	Vec2 operator + (const Vec2<T>& vec) const
	{
		return Vec2(vec.x + x, vec.y + y);
	}

	Vec2 operator - (const Vec2<T>& vec) const
	{
		return Vec2(x - vec.x, y - vec.y);
	}

	Vec2 operator / (const T val) const
	{
		return Vec2(x / val, y / val);
	}

	Vec2 operator * (const T val) const
	{
		return Vec2(x * val, y * val);
	}

	bool operator == (const Vec2<T>& vec) const
	{
		return x == vec.x && y == vec.y;
	}

	bool operator != (const Vec2<T>& vec) const
	{
		//Utilizo el overload del operador "==" y lo niego
		return !(this == vec);
	}

	void operator += (const Vec2<T>& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator -= (const Vec2<T>& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}


	void operator *= (const T val)
	{
		x *= val;
		y *= val;
	}

	void operator /= (const T val)
	{
		x /= val;
		y /= val;
	}

	Vec2 operator = (const sf::Vector2f vec) const
	{
		return Vec2(vec.x, vec.y);
	}

	Vec2 operator = (const sf::Vector2i vec) const
	{
		return Vec2(vec.x, vec.y);
	}

	float dist(const Vec2& rhs) const
	{
		return std::sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	}

	float length() const
	{
		return std::sqrt(x * x + y * y);
	}

	void normalize()
	{
		float len = length();
		if (length() > 0)
		{
			x /= len;
			y /= len;
		}
	}
};

using Vec2f = Vec2<float>;

#ifndef VECTOR2_HPP
#define VECTOR2_HPP
#include <SFML\System\Vector2.hpp>

template<typename Type>
struct Vector2 final
{
	Type x;
	Type y;

	Vector2() : x(0), y(0)
	{
	}
	Vector2(const Type& pX, const Type& pY) : x(pX), y(pY)
	{
	}
	Vector2(const Vector2<int>& right) : x((Type)right.x), y((Type)right.y)
	{
	}
	Vector2(const Vector2<float>& right) : x((Type)right.x), y((Type)right.y)
	{
	}
	Vector2(const Vector2<double>& right) : x((Type)right.x), y((Type)right.y)
	{
	}
	~Vector2()
	{
	}

	const Vector2<Type> operator=(const Vector2<Type>& right)
	{
		x = right.x;
		y = right.y;

		return *this;
	}
	const Vector2<Type> operator+=(const Vector2<Type>& right)
	{
		x += right.x;
		y += right.y;

		return *this;
	}
	const Vector2<Type> operator-=(const Vector2<Type>& right)
	{
		x -= right.x;
		y -= right.y;

		return *this;
	}
	const Vector2<Type> operator*=(const Type& coefficient)
	{
		x *= coefficient;
		y *= coefficient;

		return *this;
	}

	const Vector2<Type> operator+(const Vector2<Type>& right) const
	{
		return Vector2<Type>(x + right.x, y + right.y);
	}
	const Vector2<Type> operator-(const Vector2<Type>& right) const
	{
		return Vector2<Type>(x - right.x, y - right.y);
	}
	const Vector2<Type> operator*(const Type& coefficient) const
	{
		return Vector2<Type>(x * coefficient, y * coefficient);
	}
	
	const bool operator==(const Vector2<Type>& right) const
	{
		return (x == right.x && y == right.y);
	}

	const Vector2<Type> ReflectAround(const Vector2<Type>& vec) const
	{
		return (vec * (2 * this->dot(vec) / vec.LengthSquared()) - *this);
	}
	const Vector2<Type> ReflectOn(const Vector2<Type>& mirror) const
	{
		return (*this - mirror * (2 * this->dot(mirror) / mirror.LengthSquared()));
	}

	const Type Dot(const Vector2<Type>& right) const
	{
		return x*right.x + y*right.y;
	}
	const Vector2<Type> Orthogonal() const
	{
		return Vector2<Type>(-y, x);
	}

	void Normalize()
	{
		Type len = Length();

		if (len == 0.0f)
			return;

		Type lenDiv = 1 / len;
		x *= lenDiv;
		y *= lenDiv;
	}
	const Vector2<Type> Normalized() const
	{
		Vector2<Type> v = *this;
		v.Normalize();
		return v;
	}

	const Type Length() const
	{
		return std::sqrtf(LengthSquared());
	}
	const Type LengthSquared() const
	{
		return (x*x + y*y);
	}
	void SetLength(const Type& length)
	{
		Normalize();
		this->operator*=(length);
	}

	const sf::Vector2<Type> SFML_VEC() const
	{
		return sf::Vector2<Type>(x, y);
	}
};

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;

#endif
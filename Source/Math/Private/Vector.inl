#include <functional> // plus, minus, multiplies, divides
#include <cmath>      // sqrt
#include <iostream>   // ostream

vectorN vectorN::operator-() const
{
	return *this * -1.0f;
}

vectorN& vectorN::operator+=(const vectorN & p_rhs)
{
	return *this = *this + p_rhs;
}

vectorN& vectorN::operator-=(const vectorN & p_rhs)
{
	return *this = *this - p_rhs;
}

vectorN& vectorN::operator*=(const vectorN & p_rhs)
{
	return *this = *this * p_rhs;
}

vectorN& vectorN::operator/=(const vectorN & p_rhs)
{
	return *this = *this / p_rhs;
}

vectorN& vectorN::operator+=(const float p_rhs)
{
	return *this = *this + p_rhs;
}

vectorN& vectorN::operator-=(const float p_rhs)
{
	return *this = *this - p_rhs;
}

vectorN& vectorN::operator*=(const float p_rhs)
{
	return *this = *this * p_rhs;
}

vectorN& vectorN::operator/=(const float p_rhs)
{
	return *this = *this / p_rhs;
}

vectorN vectorN::operator+(const vectorN & p_rhs) const
{
	return concat(*this, p_rhs, std::plus<>{});
}

vectorN vectorN::operator-(const vectorN & p_rhs) const
{
	return concat(*this, p_rhs, std::minus<>{});
}

vectorN vectorN::operator*(const vectorN & p_rhs) const
{
	return concat(*this, p_rhs, std::multiplies<>{});
}

vectorN vectorN::operator/(const vectorN & p_rhs) const
{
	return concat(*this, p_rhs, std::divides<>{});
}

vectorN vectorN::operator+(const float p_rhs) const
{
	return concat(*this, p_rhs, std::plus<>{});
}

vectorN vectorN::operator-(const float p_rhs) const
{
	return concat(*this, p_rhs, std::minus<>{});
}

vectorN vectorN::operator*(const float p_rhs) const
{
	return concat(*this, p_rhs, std::multiplies<>{});
}

vectorN vectorN::operator/(const float p_rhs) const
{
	return concat(*this, p_rhs, std::divides<>{});
}

float vectorN::distanceSquared() const
{
	return dotP(*this);
}

float vectorN::distance() const
{
	return std::sqrt(distanceSquared());
}

vectorN vectorN::normalize() const
{
	return *this / distance();
}

void vectorN::normalized()
{
	*this = normalize();
}

const float* vectorN::toFloatPtr() const
{
	return &x;
}

float * vectorN::toFloatPtr()
{
	return &x;
}

void PrintTo(const vectorN& p_obj, std::ostream* p_os)
{
	*p_os << p_obj.toStringPtr();
}

float dotP(const vectorN& p_left, const vectorN& p_right)
{
	return p_left.dotP(p_right);
}

vectorN operator+(const float lhs, vectorN const& rhs)
{
	return rhs + lhs;
}

vectorN operator*(const float lhs, vectorN const& rhs)
{
	return rhs * lhs;
}

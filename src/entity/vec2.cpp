#include "vec2.hpp"
#include <cmath>

Vec2::Vec2(float x, float y) :
	x(x),
	y(y)
{}

Vec2::Vec2() :
	x(0),
	y(0)
{}

Vec2 Vec2::operator+=(const Vec2& v) {
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2& v) const {
	return Vec2(x+v.x, y+v.y);
}

Vec2 Vec2::operator*(float f) const {
	return Vec2(x*f, y*f);
}

float Vec2::length() const {
	return std::sqrt(x*x + y*y);
}

float Vec2::distance(Vec2 v) {
	return std::sqrt((x-v.x)*(x-v.x) + (y-v.y)*(y-v.y));
}

Vec2 Vec2::normalise() const {
	float mag = length();

	if(mag == 0) {
		return Vec2(0,0);
	} else {
		return Vec2(x/mag, y/mag);
	}
}
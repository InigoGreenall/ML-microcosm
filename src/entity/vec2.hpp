#ifndef VEC2_HPP
#define VEC2_HPP

struct Vec2 {
    float x, y;
    Vec2(float x, float y);
    Vec2();

    Vec2 operator+=(const Vec2& v);
    Vec2 operator+(const Vec2& v) const;
    Vec2 operator*(float f) const;

    float length() const;
    float distance(Vec2 v);
    Vec2 normalise() const;
};

#endif
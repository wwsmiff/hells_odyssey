#pragma once

#include <cstdint>
#include <ostream>
using std::uint32_t;

namespace HO
{
template <typename T> struct Vec2
{
  T x{}, y{};

  void operator+=(const Vec2<T> &other)
  {
    this->x += other.x;
    this->y += other.y;
  }
  void operator-=(const Vec2<T> &other)
  {
    this->x -= other.x;
    this->y -= other.y;
  }
  void operator*=(const Vec2<T> &other)
  {
    this->x *= other.x;
    this->y *= other.y;
  }
  void operator/=(const Vec2<T> &other)
  {
    this->x /= other.x;
    this->y /= other.y;
  }

  void operator+=(const T &scalar)
  {
    this->x += scalar;
    this->y += scalar;
  }
  void operator-=(const T &scalar)
  {
    this->x -= scalar;
    this->y -= scalar;
  }
  void operator*=(const T &scalar)
  {
    this->x *= scalar;
    this->y *= scalar;
  }
  void operator/=(const T &scalar)
  {
    this->x /= scalar;
    this->y /= scalar;
  }
};

template <typename T>
Vec2<T> operator+(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x + second.x, first.y + second.y};
}
template <typename T>
Vec2<T> operator-(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x - second.x, first.y - second.y};
}
template <typename T>
Vec2<T> operator*(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x * second.x, first.y * second.y};
}
template <typename T>
Vec2<T> operator/(const Vec2<T> &first, const Vec2<T> &second)
{
  return Vec2<T>{first.x / second.x, first.y / second.y};
}

template <typename T> Vec2<T> operator+(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x + scalar, first.y + scalar};
}
template <typename T> Vec2<T> operator-(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x - scalar, first.y - scalar};
}
template <typename T> Vec2<T> operator*(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x * scalar, first.y * scalar};
}
template <typename T> Vec2<T> operator/(const Vec2<T> &first, const T &scalar)
{
  return Vec2<T>{first.x / scalar, first.y / scalar};
}

template <typename T>
bool operator==(const Vec2<T> &first, const Vec2<T> &second)
{
  return first.x == second.x && first.y == second.y;
}
template <typename T>
bool operator!=(const Vec2<T> &first, const Vec2<T> &second)
{
  return first.x != second.x || first.y != second.y;
}
template <typename T> bool operator==(const Vec2<T> &vec2, const T &scalar)
{
  return vec2.x == scalar && vec2.y == scalar;
}
template <typename T> bool operator!=(const Vec2<T> &vec2, const T &scalar)
{
  return vec2.x != scalar || vec2.y != scalar;
}
}; // namespace HO

template <typename T>
std::ostream &operator<<(std::ostream &out, const HO::Vec2<T> &vec2)
{
  out << "Vec2(" << vec2.x << ',' << vec2.y << ')';
  return out;
}
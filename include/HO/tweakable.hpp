#pragma once
#include "HO/config.hpp"
#include <concepts>
#include <string>

namespace HO
{
template <typename T>
concept TweakValue = std::integral<T> || std::floating_point<T>;

template <TweakValue T> class Tweakable
{
public:
  Tweakable();
  Tweakable(const std::string &name, T value, T min, T max);
  void update();

protected:
  T mValue;
  if constexpr (Config::enable_debug_v)
  {
    std::string mName;
    T mMin, mMax;
  }
};

}; // namespace HO
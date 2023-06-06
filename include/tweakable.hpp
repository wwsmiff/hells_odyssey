#pragma once

#include "HO/config.hpp"
#include <string>

namespace HO
{

template <typename T> class Tweakable
{
public:
  if constexpr (enable_debug_v)
  {
    Tweakable(const std::string &name, T default, T min, T max);
      :mName{name},
       mValue{value},
       mMin{min},
       mMax{max}
    {
    }
  }

  if constexpr (enable_debug_v)
  {
  }

protected:
  if constexpr (enable_debug_v)
  {
      T mName;
      T mValue;
      T mMin;
      T mMax;
  }
  else
  {
      T mValue;
  }
};
}; // namespace HO
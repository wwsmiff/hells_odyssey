#pragma once
#include <cstdint>

namespace HO
{
namespace Config
{
using TweakableUInt = std::uint32_t;
using TweakableInt = std::int32_t;
using TweakableFlag = bool;
using TweakableFloat = float;

inline TweakableUInt playerBlocksize{128};
inline TweakableUInt playerHitbox{20};
inline TweakableFloat playerVelocity{6.0f};
inline TweakableFlag debugView = true;
}; // namespace Config
}; // namespace HO

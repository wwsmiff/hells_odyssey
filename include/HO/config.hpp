#pragma once
#include <cstdint>

constexpr inline bool enable_debug_v{true};

namespace HO
{
namespace Config
{
using TweakableUInt = std::uint32_t;
using TweakableInt = std::int32_t;
using TweakableFlag = bool;
using TweakableFloat = float;

inline constexpr TweakableFloat padding_v{16.0f};
inline constexpr TweakableInt joystick_deadzone_v{8000};
inline TweakableFloat playerBlocksize{160.0f};
inline TweakableFloat playerHitbox{20.0f};
inline TweakableFloat playerVelocity{0.75f};
inline TweakableFloat bulletVelocity{1.3f};
inline TweakableFloat bulletHitboxWidth{15.0f};
inline TweakableFloat bulletHitboxHeight{30.0f};
inline TweakableFlag debugView{false};
}; // namespace Config
}; // namespace HO
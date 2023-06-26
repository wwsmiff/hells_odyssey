#pragma once
#include <tweaky/value.hpp>

constexpr inline bool enable_debug_v{true};

namespace HO
{
namespace Config
{
using TweakableInt = tweaky::Value<int32_t>;
using TweakableFlag = tweaky::Value<bool>;
using TweakableFloat = tweaky::Value<float>;

inline constexpr float padding_v{16.0f};
inline constexpr int32_t game_controller_deadzone_v{8000};
inline TweakableFloat playerBlocksize{"Player size"};
inline TweakableFloat playerHitbox{"Player hitbox"};
inline TweakableFloat playerVelocity{"Player velocity"};
inline TweakableFloat bulletVelocity{"Bullet velocity"};
inline TweakableFloat bulletHitboxWidth{"Bullet width"};
inline TweakableFloat bulletHitboxHeight{"Bullet height"};
inline TweakableFlag debugView{"Debug view"};
}; // namespace Config
}; // namespace HO
#pragma once

#include "HO/deleters.hpp"
#include "HO/input_manager.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

enum JoystickAxis : uint8_t
{
  JOYSTICK_RIGHT = 0,
  JOYSTICK_LEFT,
  JOYSTICK_UP,
  JOYSTICK_DOWN
};

namespace HO
{
class InputManager
{
public:
  void beginNewFrame();
  void update();
  bool wasKeyPressed(SDL_Keycode key) const;
  bool wasKeyReleased(SDL_Keycode key) const;
  bool isKeyHeld(SDL_Keycode key) const;
  bool eventOccurred(SDL_EventType type) const;
  uint32_t joysticksConnected() const;
  bool joystickAxisRight() const;
  bool joystickAxisLeft() const;
  bool joystickAxisUp() const;
  bool joystickAxisDown() const;
  bool joystickAxisNone() const;

  HO::Vec2<int32_t> joystickAxis;
  HO::Vec2<int32_t> mousePosition;

protected:
  SDL_Event mEvent;
  std::unique_ptr<SDL_Joystick, Deleters::SdlDeleter> mPrimaryJoystick{};
  std::unordered_map<SDL_Keycode, bool> mHeldKeys{};
  std::unordered_map<SDL_Keycode, bool> mPressedKeys{};
  std::unordered_map<SDL_Keycode, bool> mReleasedKeys{};
  std::array<bool, 4> mJoystickAxis{};
  uint32_t mJoysticksConnected{};
};
}; // namespace HO
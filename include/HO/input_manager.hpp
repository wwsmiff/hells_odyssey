#pragma once

#include "HO/deleters.hpp"
#include "HO/input_manager.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
  InputManager();

  void beginNewFrame();
  void update();
  bool wasKeyPressed(SDL_Keycode key) const;
  bool wasKeyReleased(SDL_Keycode key) const;
  bool isKeyHeld(SDL_Keycode key) const;

  bool wasButtonPressed(uint8_t key) const;
  bool wasButtonReleased(uint8_t key) const;
  bool isButtonHeld(uint8_t key) const;

  bool eventOccurred(SDL_EventType type) const;
  uint32_t gameControllersConnected() const;
  bool gameControllerAxisRight() const;
  bool gameControllerAxisLeft() const;
  bool gameControllerAxisUp() const;
  bool gameControllerAxisDown() const;

  HO::Vec2<int32_t> gameControllerAxis;
  HO::Vec2<int32_t> mousePosition;
  std::unordered_map<SDL_EventType, std::function<void(void)>> callbacks;

protected:
  SDL_Event mEvent;
  std::unique_ptr<SDL_GameController, Deleters::SdlDeleter>
      mPrimaryGameController{};
  std::unordered_set<SDL_Keycode> mHeldKeys{};
  std::unordered_set<SDL_Keycode> mPressedKeys{};
  std::unordered_set<SDL_Keycode> mReleasedKeys{};
  std::unordered_set<uint8_t> mHeldButtons{};
  std::unordered_set<uint8_t> mPressedButtons{};
  std::unordered_set<uint8_t> mReleasedButtons{};
  std::array<bool, 4> mGameControllerAxis{};
  uint32_t mJoysticksConnected{};
};
}; // namespace HO
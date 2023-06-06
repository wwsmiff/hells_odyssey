#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>

namespace HO
{
class InputManager
{
public:
  InputManager();

  void beginNewFrame();
  void update();
  bool wasKeyPressed(SDL_Keycode key);
  bool wasKeyReleased(SDL_Keycode key);
  bool isKeyHeld(SDL_Keycode key);
  bool eventOccurred(SDL_EventType type);

protected:
  SDL_Event mEvent;
  std::unordered_map<SDL_Keycode, bool> mHeldKeys;
  std::unordered_map<SDL_Keycode, bool> mPressedKeys;
  std::unordered_map<SDL_Keycode, bool> mReleasedKeys;
};
}; // namespace HO

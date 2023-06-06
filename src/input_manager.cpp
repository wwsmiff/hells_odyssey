#include "HO/input_manager.hpp"
#include <SDL.h>

namespace HO
{
void InputManager::beginNewFrame()
{
  this->_pressedKeys.clear();
  this->_releasedKeys.clear();

  SDL_PollEvent(&(this->mEvent));

  if (this->mEvent.type == SDL_KEYDOWN)
  {
    this->mPressedKeys[this->mEvent.key.keysym.sym] = true;
    this->mHeldKeys[this->mEvent.key.keysym.sym] = true;
  }
  else if (this->mEvent.type == SDL_KEYUP)
  {
    this->mReleasedKeys[this->mEvent.key.keysym.sym] = true;
    this->mHeldKeys[this->mEvent.key.keysym.sym] = false;
  }
}

bool InputManager::wasKeyPressed(SDL_Keycode key)
{
  return this->mPressedKeys[key];
}
bool InputManager::wasKeyReleased(SDL_Keycode key)
{
  return this->mReleasedKeys[key];
}
bool InputManager::isKeyHeld(SDL_Keycode key) { return this->mHeldKeys[key]; }

bool InputManager::eventOccurred(SDL_EventType type)
{
  return (this->mEvent.type == type);
}

}; // namespace HO
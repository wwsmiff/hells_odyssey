#include "HO/input_manager.hpp"
#include "HO/config.hpp"
#include <SDL.h>
#include <iostream>

namespace HO
{
InputManager::InputManager()
{
  this->mPrimaryGameController =
      std::unique_ptr<SDL_GameController, Deleters::SdlDeleter>{
          SDL_GameControllerOpen(0)};

  SDL_JoystickEventState(SDL_ENABLE);
}

void InputManager::beginNewFrame()
{
  SDL_GetMouseState(&(this->mousePosition.x), &(this->mousePosition.y));

  this->gameControllerAxis.x = SDL_GameControllerGetAxis(
      this->mPrimaryGameController.get(), SDL_CONTROLLER_AXIS_LEFTX);

  this->gameControllerAxis.y = SDL_GameControllerGetAxis(
      this->mPrimaryGameController.get(), SDL_CONTROLLER_AXIS_LEFTY);

  this->mPressedKeys.clear();
  this->mReleasedKeys.clear();
  this->mPressedButtons.clear();
  this->mReleasedButtons.clear();

  while (SDL_PollEvent(&(this->mEvent)))
  {
    for (const auto &callback : callbacks)
    {
      if (this->mEvent.type == callback.first)
      {
        if (callback.second)
          callback.second();
      }
    }

    switch (this->mEvent.type)
    {
    case SDL_KEYDOWN:
      this->mPressedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.insert(this->mEvent.key.keysym.sym);
      break;
    case SDL_KEYUP:
      this->mReleasedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.erase(this->mEvent.key.keysym.sym);
      break;

    case SDL_CONTROLLERBUTTONDOWN:
      this->mPressedButtons.insert(this->mEvent.cbutton.button);
      this->mHeldButtons.insert(this->mEvent.cbutton.button);
      break;
    case SDL_CONTROLLERBUTTONUP:
      this->mReleasedButtons.insert(this->mEvent.cbutton.button);
      this->mHeldButtons.erase(this->mEvent.cbutton.button);
      break;
    default:
      break;
    }
  }
}

bool InputManager::wasKeyPressed(SDL_Keycode key) const
{
  return this->mPressedKeys.contains(key);
}
bool InputManager::wasKeyReleased(SDL_Keycode key) const
{

  return this->mReleasedKeys.contains(key);
}
bool InputManager::isKeyHeld(SDL_Keycode key) const
{
  return this->mHeldKeys.contains(key);
}

bool InputManager::wasButtonPressed(uint8_t key) const
{
  return this->mPressedButtons.contains(key);
}
bool InputManager::wasButtonReleased(uint8_t key) const
{

  return this->mReleasedButtons.contains(key);
}
bool InputManager::isButtonHeld(uint8_t key) const
{
  return this->mHeldButtons.contains(key);
}

bool InputManager::eventOccurred(SDL_EventType type) const
{
  return (this->mEvent.type == type);
}

bool InputManager::gameControllerAxisRight() const
{
  return this->mGameControllerAxis.at(JOYSTICK_RIGHT);
}
bool InputManager::gameControllerAxisLeft() const
{
  return this->mGameControllerAxis.at(JOYSTICK_LEFT);
}
bool InputManager::gameControllerAxisUp() const
{
  return this->mGameControllerAxis.at(JOYSTICK_UP);
}
bool InputManager::gameControllerAxisDown() const
{
  return this->mGameControllerAxis.at(JOYSTICK_DOWN);
}

uint32_t InputManager::gameControllersConnected() const
{
  return this->mJoysticksConnected;
}
}; // namespace HO
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
  this->mPressedKeys.clear();
  this->mReleasedKeys.clear();
  this->mPressedButtons.clear();
  this->mReleasedButtons.clear();

  SDL_GetMouseState(&(this->mousePosition.x), &(this->mousePosition.y));

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

    if (this->mEvent.type == SDL_KEYDOWN)
    {
      this->mPressedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.insert(this->mEvent.key.keysym.sym);
    }
    if (this->mEvent.type == SDL_KEYUP)
    {
      this->mReleasedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.erase(this->mEvent.key.keysym.sym);
    }

    if (this->mEvent.type == SDL_CONTROLLERBUTTONDOWN)
    {
      this->mPressedButtons.insert(this->mEvent.cbutton.button);
      this->mHeldButtons.insert(this->mEvent.cbutton.button);
    }
    if (this->mEvent.type == SDL_CONTROLLERBUTTONUP)
    {
      this->mReleasedButtons.insert(this->mEvent.cbutton.button);
      this->mHeldButtons.erase(this->mEvent.cbutton.button);
    }

    if (this->mEvent.type == SDL_CONTROLLERAXISMOTION)
    {
      if (SDL_GameControllerGetAxis(this->mPrimaryGameController.get(),
                                    SDL_CONTROLLER_AXIS_LEFTX) <
          -HO::Config::gameController_deadzone_v)
      {
        this->mGameControllerAxis[JOYSTICK_RIGHT] = false;
        this->mGameControllerAxis[JOYSTICK_LEFT] = true;
      }
      else if (SDL_GameControllerGetAxis(this->mPrimaryGameController.get(),
                                         SDL_CONTROLLER_AXIS_LEFTX) >
               HO::Config::gameController_deadzone_v)
      {
        this->mGameControllerAxis[JOYSTICK_LEFT] = false;
        this->mGameControllerAxis[JOYSTICK_RIGHT] = true;
      }
      else
      {
        this->mGameControllerAxis[JOYSTICK_RIGHT] = false;
        this->mGameControllerAxis[JOYSTICK_LEFT] = false;
      }

      if (SDL_GameControllerGetAxis(this->mPrimaryGameController.get(),
                                    SDL_CONTROLLER_AXIS_LEFTY) <
          -HO::Config::gameController_deadzone_v)
      {
        // printf("UP\n");
        this->mGameControllerAxis[JOYSTICK_DOWN] = false;
        this->mGameControllerAxis[JOYSTICK_UP] = true;
      }
      else if (SDL_GameControllerGetAxis(this->mPrimaryGameController.get(),
                                         SDL_CONTROLLER_AXIS_LEFTY) >
               HO::Config::gameController_deadzone_v)
      {
        // printf("DOWN\n");
        this->mGameControllerAxis[JOYSTICK_UP] = false;
        this->mGameControllerAxis[JOYSTICK_DOWN] = true;
      }
      else
      {
        this->mGameControllerAxis[JOYSTICK_UP] = false;
        this->mGameControllerAxis[JOYSTICK_DOWN] = false;
      }
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

bool InputManager::gameControllerAxisNone() const
{
  return (!this->mGameControllerAxis.at(JOYSTICK_DOWN) &&
          !this->mGameControllerAxis.at(JOYSTICK_UP) &&
          !this->mGameControllerAxis.at(JOYSTICK_LEFT) &&
          !this->mGameControllerAxis.at(JOYSTICK_RIGHT));
}

uint32_t InputManager::gameControllersConnected() const
{
  return this->mJoysticksConnected;
}
}; // namespace HO
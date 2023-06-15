#include "HO/input_manager.hpp"
#include "HO/config.hpp"
#include <SDL.h>
#include <iostream>

namespace HO
{
void InputManager::beginNewFrame()
{
  this->mPressedKeys.clear();
  this->mReleasedKeys.clear();

  SDL_GetMouseState(&(this->mousePosition.x), &(this->mousePosition.y));

  this->mJoysticksConnected = 0;
  if (!this->mPrimaryJoystick)
  {
    SDL_JoystickEventState(SDL_DISABLE);
    this->mPrimaryJoystick =
        std::unique_ptr<SDL_Joystick, Deleters::SdlDeleter>{
            SDL_JoystickOpen(mJoysticksConnected)};
  }
  else if (this->mPrimaryJoystick)
  {
    SDL_JoystickEventState(SDL_ENABLE);
    this->mJoysticksConnected++;
  }

  if (SDL_PollEvent(&(this->mEvent)))
  {
    if (this->mEvent.type == SDL_KEYDOWN)
    {
      this->mPressedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.insert(this->mEvent.key.keysym.sym);
    }
    else if (this->mEvent.type == SDL_KEYUP)
    {
      this->mReleasedKeys.insert(this->mEvent.key.keysym.sym);
      this->mHeldKeys.erase(this->mEvent.key.keysym.sym);
    }

    if (this->mPrimaryJoystick)
    {
      if (this->mEvent.type == SDL_JOYAXISMOTION)
      {
        if (this->mEvent.jaxis.which == 0) /* First Joystick */
        {
          if (this->mEvent.jaxis.axis == 0) /* Horizontal axis */
          {
            if (this->mEvent.jaxis.value < -HO::Config::joystick_deadzone_v)
            {
              this->mJoystickAxis[JOYSTICK_RIGHT] = false;
              this->mJoystickAxis[JOYSTICK_LEFT] = true;
            }
            else if (this->mEvent.jaxis.value > HO::Config::joystick_deadzone_v)
            {
              this->mJoystickAxis[JOYSTICK_LEFT] = false;
              this->mJoystickAxis[JOYSTICK_RIGHT] = true;
            }
            else
            {
              this->mJoystickAxis[JOYSTICK_RIGHT] = false;
              this->mJoystickAxis[JOYSTICK_LEFT] = false;
            }
          }

          else if (this->mEvent.jaxis.axis == 1) /* Vertical axis */
          {
            if (this->mEvent.jaxis.value < -HO::Config::joystick_deadzone_v)
            {
              this->mJoystickAxis[JOYSTICK_DOWN] = false;
              this->mJoystickAxis[JOYSTICK_UP] = true;
            }
            else if (this->mEvent.jaxis.value > HO::Config::joystick_deadzone_v)
            {
              this->mJoystickAxis[JOYSTICK_UP] = false;
              this->mJoystickAxis[JOYSTICK_DOWN] = true;
            }
            else
            {
              this->mJoystickAxis[JOYSTICK_UP] = false;
              this->mJoystickAxis[JOYSTICK_DOWN] = false;
            }
          }
        }
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

bool InputManager::eventOccurred(SDL_EventType type) const
{
  return (this->mEvent.type == type);
}

bool InputManager::joystickAxisRight() const
{
  return this->mJoystickAxis.at(JOYSTICK_RIGHT);
}
bool InputManager::joystickAxisLeft() const
{
  return this->mJoystickAxis.at(JOYSTICK_LEFT);
}
bool InputManager::joystickAxisUp() const
{
  return this->mJoystickAxis.at(JOYSTICK_UP);
}
bool InputManager::joystickAxisDown() const
{
  return this->mJoystickAxis.at(JOYSTICK_DOWN);
}

bool InputManager::joystickAxisNone() const
{
  return (!this->mJoystickAxis.at(JOYSTICK_DOWN) &&
          !this->mJoystickAxis.at(JOYSTICK_UP) &&
          !this->mJoystickAxis.at(JOYSTICK_LEFT) &&
          !this->mJoystickAxis.at(JOYSTICK_RIGHT));
}

uint32_t InputManager::joysticksConnected() const
{
  return this->mJoysticksConnected;
}

}; // namespace HO
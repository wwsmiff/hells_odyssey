#include "HO/player.hpp"
#include "HO/config.hpp"
#include "HO/input_manager.hpp"
#include <SDL.h>
#include <iostream>

namespace HO
{
void Player::handleEvents(const InputManager &inputManager)
{
  if (inputManager.wasKeyPressed(SDLK_d))
  {
    this->mPlayerLeft = false;
    this->mPlayerRight = true;
    this->mPlayerDirection.x = 1;
  }
  if (inputManager.wasKeyPressed(SDLK_a))
  {
    this->mPlayerRight = false;
    this->mPlayerLeft = true;
    this->mPlayerDirection.x = -1;
  }
  if (inputManager.wasKeyPressed(SDLK_w))
  {
    this->mPlayerDown = false;
    this->mPlayerUp = true;
    this->mPlayerDirection.y = -1;
  }
  if (inputManager.wasKeyPressed(SDLK_s))
  {
    this->mPlayerUp = false;
    this->mPlayerDown = true;
    this->mPlayerDirection.y = 1;
  }

  if (inputManager.wasKeyReleased(SDLK_d))
    this->mPlayerRight = false;
  if (inputManager.wasKeyReleased(SDLK_a))
    this->mPlayerLeft = false;

  if (inputManager.wasKeyReleased(SDLK_w))
    this->mPlayerUp = false;
  if (inputManager.wasKeyReleased(SDLK_s))
    this->mPlayerDown = false;

  if (inputManager.wasKeyReleased(SDLK_a) &&
      inputManager.wasKeyReleased(SDLK_d))
    this->mPlayerDirection.x = 0;
  if (inputManager.wasKeyReleased(SDLK_w) &&
      inputManager.wasKeyReleased(SDLK_s))
    this->mPlayerDirection.y = 0;

  /* Joystick support */
  if (inputManager.joysticksConnected() > 0)
  {
    if (inputManager.eventOccurred(SDL_JOYAXISMOTION))
    {
      if (inputManager.joystickAxisUp())
      {
        this->mPlayerDown = false;
        this->mPlayerUp = true;
        this->mPlayerDirection.y = -1;
      }
      else if (inputManager.joystickAxisDown())
      {
        this->mPlayerUp = false;
        this->mPlayerDown = true;
        this->mPlayerDirection.y = 1;
      }
      else
      {
        this->mPlayerUp = false;
        this->mPlayerDown = false;
        this->mPlayerDirection.y = 0;
      }

      if (inputManager.joystickAxisLeft())
      {
        this->mPlayerRight = false;
        this->mPlayerLeft = true;
        this->mPlayerDirection.x = -1;
      }
      else if (inputManager.joystickAxisRight())
      {
        this->mPlayerLeft = false;
        this->mPlayerRight = true;
        this->mPlayerDirection.x = 1;
      }
      else
      {
        this->mPlayerLeft = false;
        this->mPlayerRight = false;
        this->mPlayerDirection.x = 0;
      }
    }
  }
}

void Player::update(float delta)
{
  this->mPlayerDirection.normalize();

  if (this->mPlayerLeft)
  {
    if ((this->mPosition.x - Config::padding_v) > this->mHorizontalBounds.x)
      this->move(HO::Vec2<float>{
          (delta * this->mPlayerDirection.x * HO::Config::playerVelocity), 0});
  }
  if (this->mPlayerRight)
  {
    if ((this->mPosition.x + this->mSize.x + Config::padding_v) <
        this->mHorizontalBounds.y)
      this->move(Vec2<float>{
          (delta * this->mPlayerDirection.x * HO::Config::playerVelocity), 0});
  }
  if (this->mPlayerUp)
  {
    if (this->mPosition.y > (this->mVerticalBounds.x + Config::padding_v))
      this->move(Vec2<float>{
          0, (delta * this->mPlayerDirection.y * HO::Config::playerVelocity)});
  }
  if (this->mPlayerDown)
  {
    if ((this->mPosition.y + this->mSize.y + Config::padding_v) <
        this->mVerticalBounds.y)
      this->move(Vec2<float>{
          0, (delta * this->mPlayerDirection.y * HO::Config::playerVelocity)});
  }
}

void Player::setBounds(const HO::Vec2<uint32_t> &horizontal,
                       const HO::Vec2<uint32_t> &vertical)
{
  this->mHorizontalBounds = horizontal;
  this->mVerticalBounds = vertical;
}
void Player::setHorizontalBounds(const HO::Vec2<uint32_t> &horizontal)
{
  this->mHorizontalBounds = horizontal;
}
void Player::setVerticalBounds(const HO::Vec2<uint32_t> &vertical)
{
  this->mVerticalBounds = vertical;
}
}; // namespace HO

#include "HO/player.hpp"
#include "HO/config.hpp"
#include <SDL.h>
#include <iostream>

namespace HO
{
void Player::handleEvents(SDL_Event &event)
{
  if (event.type == SDL_KEYDOWN)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_d:
      this->mPlayerLeft = false;
      this->mPlayerRight = true;
      this->mPlayerDirection.x = 1;
      break;
    case SDLK_a:
      this->mPlayerRight = false;
      this->mPlayerLeft = true;
      this->mPlayerDirection.x = -1;
      break;
    case SDLK_w:
      this->mPlayerDown = false;
      this->mPlayerUp = true;
      this->mPlayerDirection.y = -1;
      break;
    case SDLK_s:
      this->mPlayerUp = false;
      this->mPlayerDown = true;
      this->mPlayerDirection.y = 1;
      break;
    }
  }

  if (event.type == SDL_KEYUP)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_d:
      this->mPlayerRight = false;
      break;
    case SDLK_a:
      this->mPlayerLeft = false;
      break;
    case SDLK_w:
      this->mPlayerUp = false;
      break;
      break;
    case SDLK_s:
      this->mPlayerDown = false;
      break;
    default:
      this->mPlayerDirection = {0, 0};
      break;
    }

    if (event.key.keysym.sym == SDLK_a && event.key.keysym.sym == SDLK_d)
      this->mPlayerDirection.x = 0;
    if (event.key.keysym.sym == SDLK_w && event.key.keysym.sym == SDLK_s)
      this->mPlayerDirection.y = 0;
  }

  /* Joystick support */
  if (event.type == SDL_JOYAXISMOTION)
  {
    if (event.jaxis.which == 0)
    {
      if (event.jaxis.axis == 0)
      {
        if (event.jaxis.value < -HO::Config::joystick_deadzone_v)
        {
          this->mPlayerRight = false;
          this->mPlayerLeft = true;
          this->mPlayerDirection.x = -1;
        }
        else if (event.jaxis.value > HO::Config::joystick_deadzone_v)
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

      else if (event.jaxis.axis == 1)
      {
        if (event.jaxis.value < -HO::Config::joystick_deadzone_v)
        {
          this->mPlayerDown = false;
          this->mPlayerUp = true;
          this->mPlayerDirection.y = -1;
        }
        else if (event.jaxis.value > HO::Config::joystick_deadzone_v)
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
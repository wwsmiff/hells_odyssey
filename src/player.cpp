#include "HO/player.hpp"
#include "HO/config.hpp"
#include "HO/input_manager.hpp"
#include "HO/rgba.hpp"
#include <SDL.h>
#include <chrono>
#include <iostream>

namespace HO
{
Player::Player(const Vec2<float> &position, const Vec2<float> &size)
    : Entity(position, size),
      mLeftThruster{25,
                    Vec2<float>{(this->mPosition.x + 58),
                                (this->mPosition.y + this->mSize.y - 32)},
                    Vec2<float>{-1.0f, 1.0f},
                    Vec2<float>{0.0f, 4.0f},
                    25.0f,
                    Rgba{200, 200, 200, 255}},
      mRightThruster{25,
                     Vec2<float>{(this->mPosition.x + this->mSize.x - 70),
                                 (this->mPosition.y + this->mSize.y - 32)},
                     Vec2<float>{-1.0f, 1.0f},
                     Vec2<float>{0.0f, 4.0f},
                     25.0f,
                     Rgba{200, 200, 200, 255}}
{
  for (size_t i = 0; i < 200; ++i)
  {
    this->mBullets.emplace_back(
        DOUBLE, Vec2<float>{((this->mPosition.x + (this->mSize.x / 2)) -
                             (Config::bulletHitboxWidth / 2)),
                            position.y + 50});
  }
}

void Player::move(const Vec2<float> &offset)
{
  this->mPosition.x += offset.x;
  this->mPosition.y += offset.y;
  this->mRenderRect.x = static_cast<int32_t>(this->mPosition.x);
  this->mRenderRect.y = static_cast<int32_t>(this->mPosition.y);
  this->mHitbox.x =
      static_cast<int32_t>((this->mRenderRect.x + (this->mRenderRect.w / 2)) -
                           (this->mHitbox.w / 2));
  this->mHitbox.y =
      static_cast<int32_t>((this->mRenderRect.y + (this->mRenderRect.h / 2)) -
                           (this->mHitbox.h / 2));

  for (auto &bullet : this->mBullets)
    bullet.setOrigin(Vec2<float>{
        ((this->mPosition.x + (this->mSize.x / 2)) - (bullet.getSize().x / 2)),
        this->mPosition.y + 50});

  mLeftThruster.setOrigin(Vec2<float>{
      (this->mPosition.x + 58), (this->mPosition.y + this->mSize.y - 32)});
  mRightThruster.setOrigin(
      Vec2<float>{(this->mPosition.x + this->mSize.x - 70),
                  (this->mPosition.y + this->mSize.y - 32)});
}

void Player::handleEvents(const InputManager &inputManager)
{
  if (inputManager.isKeyHeld(SDLK_d))
  {
    this->mPlayerLeft = false;
    this->mPlayerRight = true;

    this->mPlayerDirection.x = 1;
  }
  if (inputManager.isKeyHeld(SDLK_a))
  {
    this->mPlayerRight = false;
    this->mPlayerLeft = true;
    this->mPlayerDirection.x = -1;
  }
  if (inputManager.isKeyHeld(SDLK_w))
  {
    this->mPlayerDown = false;
    this->mPlayerUp = true;
    this->mPlayerDirection.y = -1;
  }
  if (inputManager.isKeyHeld(SDLK_s))
  {
    this->mPlayerUp = false;
    this->mPlayerDown = true;
    this->mPlayerDirection.y = 1;
  }

  if (inputManager.wasKeyPressed(SDLK_SPACE))
  {
    for (size_t i = 0; i < this->mBullets.size(); ++i)
    {
      if (!this->mBullets[i].active())
      {
        this->mBullets[i].fire();
        break;
      }
      break;
    }
  }
  if (inputManager.isKeyHeld(SDLK_SPACE))
  {
    static size_t i{0};
    if (!this->mBullets[i].active())
      this->mBullets[i].fire();

    if (this->mElapsedTime > 75)
    {
      i = (i < this->mBullets.size()) ? i + 1 : 0;
      this->mElapsedTime = 0;
    }
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
  this->mElapsedTime += delta;

  this->mLeftThruster.update(delta);
  this->mRightThruster.update(delta);

  this->mPlayerDirection.normalize();

  if (this->mPlayerLeft)
  {
    if ((this->mPosition.x - Config::padding_v) > this->mHorizontalBounds.x)
    {
      this->move(HO::Vec2<float>{
          (delta * this->mPlayerDirection.x * HO::Config::playerVelocity), 0});
      this->mRightThruster.moveParticles(Vec2<float>{
          delta * this->mPlayerDirection.x * Config::playerVelocity, 0});
      this->mLeftThruster.moveParticles(Vec2<float>{
          delta * this->mPlayerDirection.x * Config::playerVelocity, 0});

      for (auto &bullet : this->mBullets)
        bullet.move(Vec2<float>{delta * this->mPlayerDirection.x *
                                    (Config::playerVelocity - 0.3f),
                                0});
    }
  }
  if (this->mPlayerRight)
  {
    if ((this->mPosition.x + this->mSize.x + Config::padding_v) <
        this->mHorizontalBounds.y)
    {
      this->move(Vec2<float>{
          (delta * this->mPlayerDirection.x * HO::Config::playerVelocity), 0});
      this->mRightThruster.moveParticles(Vec2<float>{
          delta * this->mPlayerDirection.x * Config::playerVelocity, 0});
      this->mLeftThruster.moveParticles(Vec2<float>{
          delta * this->mPlayerDirection.x * Config::playerVelocity, 0});

      for (auto &bullet : this->mBullets)
        bullet.move(Vec2<float>{delta * this->mPlayerDirection.x *
                                    (Config::playerVelocity - 0.3f),
                                0});
    }
  }
  if (this->mPlayerUp)
  {
    if (this->mPosition.y > (this->mVerticalBounds.x + Config::padding_v))
    {
      this->move(Vec2<float>{
          0, (delta * this->mPlayerDirection.y * HO::Config::playerVelocity)});
      this->mRightThruster.moveParticles(Vec2<float>{
          0, delta * this->mPlayerDirection.y * Config::playerVelocity});
      this->mLeftThruster.moveParticles(Vec2<float>{
          0, delta * this->mPlayerDirection.y * Config::playerVelocity});
    }
  }
  if (this->mPlayerDown)
  {
    if ((this->mPosition.y + this->mSize.y + Config::padding_v) <
        this->mVerticalBounds.y)
    {
      this->move(Vec2<float>{
          0, (delta * this->mPlayerDirection.y * HO::Config::playerVelocity)});

      this->mRightThruster.moveParticles(Vec2<float>{
          0, delta * this->mPlayerDirection.y * Config::playerVelocity});
      this->mLeftThruster.moveParticles(Vec2<float>{
          0, delta * this->mPlayerDirection.y * Config::playerVelocity});
    }
  }

  for (auto &bullet : this->mBullets)
    bullet.update(delta);
}

void Player::render(SDL_Renderer *renderer)
{
  for (auto &bullet : this->mBullets)
    bullet.render(renderer);

  if (Config::debugView)
  {
    SDL_SetRenderDrawColor(renderer, this->mColor.r, this->mColor.g,
                           this->mColor.b, this->mColor.a);
    SDL_RenderDrawRect(renderer, &(this->mRenderRect));
    SDL_SetRenderDrawColor(renderer, 255, 68, 78, 255);
    SDL_RenderDrawRect(renderer, &(this->mHitbox));
  }
  else
  {
    SDL_RenderCopy(renderer, this->mTexture.get(), nullptr,
                   &(this->mRenderRect));
  }

  this->mLeftThruster.render(renderer);
  this->mRightThruster.render(renderer);
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

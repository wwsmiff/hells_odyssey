#include "HO/bullet.hpp"
#include "HO/config.hpp"
#include <SDL.h>

namespace HO
{

Bullet::Bullet() {}

Bullet::Bullet(uint8_t type, const Vec2<float> &origin)
    : mType{type}, mActive{false}, mPosition{origin}, mOrigin{origin}
{
  if (this->mType == CLASSIC)
  {
    this->mSize = {25, 25};
    this->mColor = {0, 255, 255, 255};
    /* TODO: Load texture */
  }

  this->mHitbox = {static_cast<int32_t>(this->mPosition.x),
                   static_cast<int32_t>(this->mPosition.y),
                   static_cast<int32_t>(this->mSize.x),
                   static_cast<int32_t>(this->mSize.y)};
}

void Bullet::update(float delta)
{
  if (this->mActive)
  {
    if (this->mPosition.y > 5)
      this->mPosition.y -= (delta * Config::bulletVelocity);
    else
    {
      this->mPosition = this->mOrigin;
      this->active(false);
    }
  }
  else
    this->mPosition = this->mOrigin;

  this->mHitbox.x = this->mPosition.x;
  this->mHitbox.y = this->mPosition.y;
}
bool Bullet::active() const { return this->mActive; }
void Bullet::active(bool state) { this->mActive = state; }
void Bullet::setOrigin(const Vec2<float> &origin) { this->mOrigin = origin; }
void Bullet::setType(BulletType type) { this->mType = type; }
void Bullet::fire()
{
  if (!this->mActive)
    this->active(true);
}
void Bullet::render(SDL_Renderer *renderer)
{
  if (Config::debugView)
  {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderDrawRect(renderer, &(this->mHitbox));
  }
  if (this->mActive)
  {
    SDL_RenderCopy(renderer, this->mTexture.get(), nullptr, &(this->mHitbox));
  }
}

Vec2<float> Bullet::getSize() const { return this->mSize; }
Vec2<float> Bullet::getPosition() const { return this->mPosition; }
Vec2<float> Bullet::getOrigin() const { return this->mOrigin; }

}; // namespace HO
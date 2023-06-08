#include "HO/bullet.hpp"
#include "HO/config.hpp"
#include <SDL.h>

namespace HO
{

Bullet::Bullet() {}

Bullet::Bullet(uint8_t type, const Vec2<float> &origin)
    : mType{type}, mActive{false}, mOrigin{origin}, mHitboxes{}
{
  if (this->mType == CLASSIC)
  {
    this->mHitboxes.clear();
    this->mSize = {Config::bulletHitbox, Config::bulletHitbox};
    this->mColor = {0, 255, 255, 255};
    this->mHitboxes.push_back(SDL_Rect{static_cast<int32_t>(this->mOrigin.x),
                                       static_cast<int32_t>(this->mOrigin.y),
                                       static_cast<int32_t>(this->mSize.x),
                                       static_cast<int32_t>(this->mSize.y)});
  }
  else if (this->mType == DOUBLE)
  {
    this->mHitboxes.clear();
    this->mSize = {Config::bulletHitbox, Config::bulletHitbox};
    this->mColor = {0, 60, 255, 255};
    this->mHitboxes.push_back(
        SDL_Rect{static_cast<int32_t>((this->mOrigin.x - (this->mSize.x * 2))),
                 static_cast<int32_t>(this->mOrigin.y),
                 static_cast<int32_t>(this->mSize.x),
                 static_cast<int32_t>(this->mSize.y)});
    this->mHitboxes.push_back(
        SDL_Rect{static_cast<int32_t>((this->mOrigin.x + (this->mSize.x * 2))),
                 static_cast<int32_t>(this->mOrigin.y),
                 static_cast<int32_t>(this->mSize.x),
                 static_cast<int32_t>(this->mSize.y)});
  }
  /* TODO: Load texture */
}

void Bullet::update(float delta)
{
  if (this->mActive)
  {
    for (auto &hitbox : this->mHitboxes)
    {
      if (hitbox.y > 5)
        hitbox.y -= static_cast<int32_t>((delta * Config::bulletVelocity));
      else
      {
        hitbox.x = static_cast<int32_t>(this->mOrigin.x);
        hitbox.y = static_cast<int32_t>(this->mOrigin.y);
        this->active(false);
      }
    }
  }
  else
  {
    if (this->mType == CLASSIC)
    {
      this->mHitboxes[0].x = this->mOrigin.x;
      this->mHitboxes[0].y = this->mOrigin.y;
    }
    if (this->mType == DOUBLE)
    {
      this->mHitboxes[0].x = this->mOrigin.x - (Config::bulletHitbox * 2);
      this->mHitboxes[1].x = this->mOrigin.x + (Config::bulletHitbox * 2);
      this->mHitboxes[0].y = this->mHitboxes[1].y = this->mOrigin.y;
    }
  }
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
    SDL_SetRenderDrawColor(renderer, this->mColor.r, this->mColor.g,
                           this->mColor.b, this->mColor.a);
    for (const auto &hitbox : this->mHitboxes)
      SDL_RenderDrawRect(renderer, &(hitbox));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, this->mOrigin.x, this->mOrigin.y);
  }
  if (this->mActive)
  {
    for (const auto &hitbox : this->mHitboxes)
    {
      SDL_RenderCopy(renderer, this->mTexture.get(), nullptr, &(hitbox));
    }
  }
}

Vec2<float> Bullet::getSize() const { return this->mSize; }
Vec2<float> Bullet::getOrigin() const { return this->mOrigin; }

}; // namespace HO
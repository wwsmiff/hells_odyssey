#include "HO/bullet.hpp"
#include "HO/config.hpp"
#include <SDL.h>
#include <iostream>

namespace HO
{

Bullet::Bullet() {}

Bullet::Bullet(uint8_t type, const Vec2<float> &origin)
    : mType{type}, mActive{false}, mOrigin{origin}, mTexture{}, mHitboxes{},
      mColor{}
{
  if (this->mType == CLASSIC)
  {
    this->mSize = {Config::bulletHitboxWidth, Config::bulletHitboxHeight};
    this->mColor = {0, 255, 255, 255};
    this->mHitboxes.push_back(
        {this->mOrigin.x, this->mOrigin.y, this->mSize.x, this->mSize.y});
  }
  else if (this->mType == DOUBLE)
  {
    this->mSize = {Config::bulletHitboxWidth, Config::bulletHitboxHeight};
    this->mColor = {0, 60, 255, 255};
    this->mHitboxes.push_back({((this->mOrigin.x - (this->mSize.x * 1.5f))),
                               (this->mOrigin.y), (this->mSize.x),
                               (this->mSize.y)});
    this->mHitboxes.push_back({((this->mOrigin.x + (this->mSize.x * 1.5f))),
                               (this->mOrigin.y), (this->mSize.x),
                               (this->mSize.y)});
  }
  /* TODO: Load texture */
}

void Bullet::update(float delta)
{
  if constexpr (enable_debug_v)
  {
    for (auto &hitbox : this->mHitboxes)
    {
      hitbox.w = HO::Config::bulletHitboxWidth;
      hitbox.h = HO::Config::bulletHitboxHeight;
    }
  }

  if (this->mActive)
  {
    if (this->mType == CLASSIC)
    {
      if (this->mHitboxes[0].y > 0)
        this->mHitboxes[0].y -= delta * Config::bulletVelocity;
      else
      {
        this->mHitboxes[0].x = this->mOrigin.x;
        this->mHitboxes[0].y = this->mOrigin.y;
        this->active(false);
      }
    }

    else if (this->mType == DOUBLE)
    {
      if (this->mHitboxes[0].y > 0 && this->mHitboxes[1].y > 0)
      {
        this->mHitboxes[0].y -= delta * Config::bulletVelocity;
        this->mHitboxes[1].y -= delta * Config::bulletVelocity;
      }
      else
      {
        this->mHitboxes[0].x = this->mOrigin.x - (this->mSize.x * 1.5f);
        this->mHitboxes[0].y = this->mOrigin.y;
        this->mHitboxes[1].x = this->mOrigin.x + (this->mSize.x * 1.5f);
        this->mHitboxes[1].y = this->mOrigin.y;
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
    else if (this->mType == DOUBLE)
    {
      this->mHitboxes[0].x = this->mOrigin.x - (this->mSize.x * 1.5);
      this->mHitboxes[1].x = this->mOrigin.x + (this->mSize.x * 1.5);
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
  if (!this->mTexture.get())
    this->mTexture.load(renderer,
                        "../assets/sprites/bullets/player_bullet_type_1.png");

  if (Config::debugView)
  {
    SDL_SetRenderDrawColor(renderer, this->mColor.r, this->mColor.g,
                           this->mColor.b, this->mColor.a);
    for (const auto &hitbox : this->mHitboxes)
      SDL_RenderDrawRectF(renderer, &(hitbox));
  }
  else
  {
    if (this->mActive)
    {
      std::vector<SDL_Vertex> vertices{};
      std::vector<int32_t> indices{};
      for (const auto &hitbox : this->mHitboxes)
      {
        const size_t offset{vertices.size()};
        vertices.push_back(SDL_Vertex{
            {static_cast<float>(hitbox.x),
             static_cast<float>(hitbox.y) + static_cast<float>(hitbox.h)},
            {255, 255, 255, this->mColor.a},
            {0.0f, 1.0f}});
        vertices.push_back(SDL_Vertex{
            {static_cast<float>(hitbox.x) + static_cast<float>(hitbox.w),
             static_cast<float>(hitbox.y + hitbox.h)},
            {255, 255, 255, this->mColor.a},
            {1.0f, 1.0f}});
        vertices.push_back(SDL_Vertex{
            {static_cast<float>(hitbox.x) + static_cast<float>(hitbox.w),
             static_cast<float>(hitbox.y)},
            {255, 255, 255, this->mColor.a},
            {1.0f, 0.0f}});
        vertices.push_back(SDL_Vertex{
            {static_cast<float>(hitbox.x), static_cast<float>(hitbox.y)},
            {255, 255, 255, this->mColor.a},
            {0.0f, 0.0f}});

        indices.push_back(offset);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset);
      }

      SDL_RenderGeometry(renderer, mTexture.get(), vertices.data(),
                         vertices.size(), indices.data(), indices.size());
    }
  }
}

void Bullet::move(Vec2<float> offset)
{
  for (auto &hitbox : this->mHitboxes)
  {
    hitbox.x += offset.x;
    hitbox.y += offset.y;
  }
}

Vec2<float> Bullet::getSize() const { return this->mSize; }
Vec2<float> Bullet::getOrigin() const { return this->mOrigin; }
}; // namespace HO
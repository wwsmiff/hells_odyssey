#include "HO/entity.hpp"

#define HO_DEBUG 1

namespace HO
{
Entity::Entity()
    : mTexture{}, mPosition{0, 0}, mSize{0, 0}, mHitbox{0, 0, 0, 0},
      mRenderRect{0, 0, 0, 0}, mColor{0}
{
}

Entity::Entity(const Vec2<uint32_t> &position, const Vec2<uint32_t> &size)
    : mTexture{}, mPosition{position}, mSize{size}, mHitbox{0, 0, 0, 0},
      mRenderRect{static_cast<int32_t>(position.x),
                  static_cast<int32_t>(position.y),
                  static_cast<int32_t>(size.x), static_cast<int32_t>(size.y)},
      mColor{0}
{
}

void Entity::setColor(const HO::Rgba &color) { this->mColor = color; }

void Entity::render(SDL_Renderer *renderer)
{
#if HO_DEBUG
  SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
  SDL_RenderDrawRect(renderer, &(this->mRenderRect));
  SDL_SetRenderDrawColor(renderer, 255, 68, 78, 255);
  SDL_RenderDrawRect(renderer, &(this->mHitbox));
#else
  SDL_RenderCopy(renderer, this->mTexture.get(), nullptr, &(this->mRenderRect));
#endif
}

void Entity::move(const Vec2<int32_t> &offset)
{
  this->mPosition.x += offset.x;
  this->mPosition.y += offset.y;
  this->mRenderRect.x = this->mPosition.x;
  this->mRenderRect.y = this->mPosition.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (this->mHitbox.w / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (this->mHitbox.h / 2);
}
void Entity::setPosition(const Vec2<uint32_t> &position)
{
  this->mPosition = position;
  this->mRenderRect.x = this->mPosition.x;
  this->mRenderRect.y = this->mPosition.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (this->mHitbox.w / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (this->mHitbox.h / 2);
}
void Entity::setSize(const Vec2<uint32_t> &size)
{
  this->mSize = size;
  this->mRenderRect.w = this->mSize.x;
  this->mRenderRect.h = this->mSize.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (this->mHitbox.w / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (this->mHitbox.h / 2);
}

void Entity::setHitbox(const Vec2<uint32_t> &size)
{
  this->mHitbox.w = size.x;
  this->mHitbox.h = size.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (size.x / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (size.y / 2);
}

void Entity::loadTexture(SDL_Renderer *renderer, const std::string &path)
{
  this->mTexture.load(renderer, path);
}

Vec2<uint32_t> Entity::getPosition() const { return this->mPosition; }
Vec2<uint32_t> Entity::getSize() const { return this->mSize; }

}; // namespace HO
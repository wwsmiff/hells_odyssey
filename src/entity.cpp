#include "HO/entity.hpp"
#include "HO/config.hpp"

namespace HO
{
Entity::Entity(const Vec2<float> &position, const Vec2<float> &size)
    : mTexture{}, mPosition{position}, mSize{size}, mHitbox{0, 0, 0, 0},
      mRenderRect{position.x, position.y, size.x, size.y}, mColor{0}
{
}

void Entity::setColor(const HO::Rgba &color) { this->mColor = color; }

void Entity::render(SDL_Renderer *renderer)
{
  if (Config::debugView)
  {
    SDL_SetRenderDrawColor(renderer, mColor.r, mColor.g, mColor.b, mColor.a);
    SDL_RenderFillRectF(renderer, &(this->mRenderRect));
  }
  else
  {
    SDL_RenderCopyF(renderer, this->mTexture.get(), nullptr,
                    &(this->mRenderRect));
  }
}

void Entity::move(const Vec2<float> &offset)
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
void Entity::setPosition(const Vec2<float> &position)
{
  this->mPosition = position;
  this->mRenderRect.x = this->mPosition.x;
  this->mRenderRect.y = this->mPosition.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (this->mHitbox.w / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (this->mHitbox.h / 2);
}
void Entity::setSize(const Vec2<float> &size)
{
  this->mSize = size;
  this->mRenderRect.w = this->mSize.x;
  this->mRenderRect.h = this->mSize.y;
  this->mHitbox.x =
      (this->mRenderRect.x + (this->mRenderRect.w / 2)) - (this->mHitbox.w / 2);
  this->mHitbox.y =
      (this->mRenderRect.y + (this->mRenderRect.h / 2)) - (this->mHitbox.h / 2);
}

void Entity::setHitbox(const Vec2<float> &size)
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

Vec2<float> Entity::getPosition() const { return this->mPosition; }
Vec2<float> Entity::getSize() const { return this->mSize; }

}; // namespace HO

#pragma once

#include "HO/rgba.hpp"
#include "HO/texture.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <cstdint>

using std::uint32_t;

namespace HO
{
class Entity
{
public:
  Entity();
  Entity(const Vec2<uint32_t> &position, const Vec2<uint32_t> &size);
  void setColor(const HO::Rgba &color);
  virtual void render(SDL_Renderer *renderer);
  void move(const Vec2<int32_t> &offset);
  void setPosition(const Vec2<uint32_t> &position);
  void setSize(const Vec2<uint32_t> &size);
  void setHitbox(const Vec2<uint32_t> &size);
  void loadTexture(SDL_Renderer *renderer, const std::string &path);

  Vec2<uint32_t> getPosition() const;
  Vec2<uint32_t> getSize() const;

protected:
  Texture mTexture;
  Vec2<uint32_t> mPosition, mSize;
  SDL_Rect mHitbox;
  SDL_Rect mRenderRect;
  Rgba mColor;
};
}; // namespace HO
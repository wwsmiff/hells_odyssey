#pragma once

#include "HO/rgba.hpp"
#include "HO/texture.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <cstdint>

using std::int32_t;
using std::uint32_t;

namespace HO
{
class Entity
{
public:
  explicit Entity(const Vec2<float> &position, const Vec2<float> &size);
  void setColor(const HO::Rgba &color);
  virtual void update(float delta) = 0;
  virtual void render(SDL_Renderer *renderer);
  void move(const Vec2<float> &offset);
  void setPosition(const Vec2<float> &position);
  void setSize(const Vec2<float> &size);
  void setHitbox(const Vec2<float> &size);
  void loadTexture(SDL_Renderer *renderer, const std::string &path);

  Vec2<float> getPosition() const;
  Vec2<float> getSize() const;

protected:
  Texture mTexture;
  Vec2<float> mPosition, mSize;
  SDL_Rect mHitbox;
  SDL_Rect mRenderRect;
  Rgba mColor;
};
}; // namespace HO
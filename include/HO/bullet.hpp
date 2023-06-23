#pragma once

#include "HO/rgba.hpp"
#include "HO/texture.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <cstdint>
#include <vector>

namespace HO
{
enum BulletType : uint8_t
{
  CLASSIC = 0,
  DOUBLE,
};

class Bullet
{
public:
  Bullet();
  Bullet(uint8_t type, const Vec2<float> &origin);
  void update(float delta);
  void move(Vec2<float> offset);
  bool active() const;
  void active(bool state);
  void render(SDL_Renderer *renderer);
  void setOrigin(const Vec2<float> &origin);
  void setType(BulletType type);
  void setSize(const Vec2<float> &size);
  void fire();

  Vec2<float> getSize() const;
  Vec2<float> getOrigin() const;
  Vec2<float> getPosition() const;

protected:
  uint8_t mType{};
  bool mActive{};
  Vec2<float> mOrigin{};
  Vec2<float> mSize{};
  Texture mTexture{};
  std::vector<SDL_FRect> mHitboxes{};
  HO::Rgba mColor{};
};
}; // namespace HO
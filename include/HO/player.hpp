#pragma once

#include "HO/entity.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>

namespace HO
{
class Player : public Entity
{
public:
  using Entity::Entity;

  void handleEvents(SDL_Event &event);
  void update(float delta);
  void setBounds(const HO::Vec2<uint32_t> &horizontal,
                 const HO::Vec2<uint32_t> &vertical);
  void setHorizontalBounds(const HO::Vec2<uint32_t> &horizontal);
  void setVerticalBounds(const HO::Vec2<uint32_t> &vertical);

protected:
  bool mPlayerUp, mPlayerDown, mPlayerLeft, mPlayerRight;
  HO::Vec2<uint32_t> mHorizontalBounds;
  HO::Vec2<uint32_t> mVerticalBounds;
  HO::Vec2<int32_t> mPlayerDirection;
};
}; // namespace HO
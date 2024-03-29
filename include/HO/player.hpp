#pragma once

#include "HO/bullet.hpp"
#include "HO/entity.hpp"
#include "HO/input_manager.hpp"
#include "HO/particles.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <vector>

namespace HO
{
class Player final : public Entity
{
public:
  explicit Player(const Vec2<float> &position, const Vec2<float> &size);

  void move(const Vec2<float> &offset) final;
  void handleEvents(InputManager &inputManager);
  void update(float delta) final;
  void render(SDL_Renderer *renderer) final;
  void setBounds(const Vec2<uint32_t> &horizontal,
                 const Vec2<uint32_t> &vertical);
  void setHorizontalBounds(const Vec2<uint32_t> &horizontal);
  void setVerticalBounds(const Vec2<uint32_t> &vertical);

protected:
  std::vector<Bullet> mBullets;
  size_t mBulletIndex{};
  bool mPlayerUp{}, mPlayerDown{}, mPlayerLeft{}, mPlayerRight{};
  Vec2<uint32_t> mHorizontalBounds{};
  Vec2<uint32_t> mVerticalBounds{};
  Vec2<float> mPlayerDirection{};
  float mElapsedTime{};
  ParticleSystem mLeftThruster;
  ParticleSystem mRightThruster;
};
}; // namespace HO
#pragma once

#include "HO/rgba.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <vector>

namespace HO
{
class Particle
{
public:
  Particle();
  Particle(const Vec2<float> &origin, const Vec2<float> &hSpread,
           const Vec2<float> &vSpread, float decayRate, const Rgba &color);
  void update(float delta);

  friend class ParticleSystem;

protected:
  Vec2<float> mOrigin{};
  Vec2<float> mPosition{};
  Vec2<float> mVelocity{};
  Vec2<float> mSize{};
  float mLifespan{};
  float mDecayRate{};
  Rgba mColor{};
};

class ParticleSystem
{
public:
  ParticleSystem(size_t noOfParticles, const Vec2<float> &origin,
                 const Vec2<float> &hSpread, const Vec2<float> &vSpread,
                 float decayRate, const Rgba &color);

  void update(float delta);
  void render(SDL_Renderer *renderer);
  void setOrigin(const Vec2<float> &origin);
  void moveParticles(Vec2<float> offset);

protected:
  std::vector<Particle> mParticles;
};

}; // namespace HO
#include "HO/particles.hpp"
#include "HO/random.hpp"
#include <iostream>

namespace HO
{

/* Implentation of `Particle` */

Particle::Particle() {}

Particle::Particle(const Vec2<float> &origin, const Vec2<float> &hSpread,
                   const Vec2<float> &vSpread, float decayRate,
                   const Rgba &color)
    : mOrigin{origin}, mPosition{origin},
      mVelocity{Random::Float(hSpread.x, hSpread.y),
                Random::Float(vSpread.x, vSpread.y)},
      mLifespan{255.0f}, mDecayRate{decayRate}, mColor{color}
{
}

void Particle::update(float delta)
{
  if (this->mLifespan > 0.0f)
  {
    this->mPosition += this->mVelocity * delta;
    this->mLifespan -= this->mDecayRate * delta;
  }
  else if (this->mLifespan <= 0.0f)
  {
    this->mPosition = this->mOrigin;
    this->mLifespan = 255.0f;
  }
}

void Particle::render(SDL_Renderer *renderer)
{
  if (this->mLifespan > 0.0f)
  {
    SDL_SetRenderDrawColor(renderer, this->mColor.r, this->mColor.g,
                           this->mColor.b,
                           static_cast<int32_t>(this->mLifespan));
    SDL_Rect tmp{static_cast<int32_t>(this->mPosition.x),
                 static_cast<int32_t>(this->mPosition.y), 10, 20};
    SDL_RenderFillRect(renderer, &tmp);
  }
}

/* End */

/* Implementation of `ParticleSystem` */

ParticleSystem::ParticleSystem(size_t noOfParticles, const Vec2<float> &origin,
                               const Vec2<float> &hSpread,
                               const Vec2<float> &vSpread, float decayRate,
                               const Rgba &color)
    : mParticles(noOfParticles)
{
  for (auto &p : this->mParticles)
    p = Particle{origin, hSpread, vSpread, decayRate, color};
}

void ParticleSystem::update(float delta)
{
  for (size_t i = 0; i < this->mParticles.size(); ++i)
  {
    if (this->mCounter > 50)
    {
      this->mParticles[i].update(delta);
      this->mCounter = 0;
    }
    else
      this->mCounter++;
  }
}

void ParticleSystem::render(SDL_Renderer *renderer)
{
  for (auto &p : this->mParticles)
    p.render(renderer);
}

void ParticleSystem::setOrigin(const Vec2<float> &origin)
{
  for (auto &p : this->mParticles)
    p.mOrigin = origin;
}

void ParticleSystem::moveParticles(Vec2<float> offset)
{
  for (auto &p : this->mParticles)
  {
    p.mPosition += offset;
  }
}

/* End */

}; // namespace HO
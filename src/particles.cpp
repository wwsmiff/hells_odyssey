#include "HO/particles.hpp"
#include "HO/random.hpp"
#include <array>
#include <iostream>
#include <vector>

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
      mSize{10, 20}, mLifespan{Random::Float(0.0f, 255.0f)},
      mDecayRate{decayRate}, mColor{color}
{
}

void Particle::update(float delta)
{
  constexpr float speed_v{0.07};
  if (this->mLifespan > 0.0f)
  {
    this->mPosition += this->mVelocity * speed_v * delta;
    this->mLifespan -= this->mDecayRate * speed_v * delta;
  }
  else if (this->mLifespan <= 0.0f)
  {
    this->mPosition = this->mOrigin;
    this->mLifespan = 255.0f;
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
  for (size_t i{0}; i < this->mParticles.size(); ++i)
    this->mParticles[i].update(delta);
}

void ParticleSystem::render(SDL_Renderer *renderer)
{
  std::vector<SDL_Vertex> vertices{};
  std::vector<int32_t> indices{};

  for (const auto &quad : this->mParticles)
  {
    const size_t offset{vertices.size()};
    if (quad.mLifespan > 0.0f)
    {
      vertices.push_back(
          SDL_Vertex{{quad.mPosition.x, quad.mPosition.y + quad.mSize.y},
                     {quad.mColor.r, quad.mColor.g, quad.mColor.b,
                      static_cast<uint8_t>(quad.mLifespan)},
                     {0, 0}});

      vertices.push_back(SDL_Vertex{
          {quad.mPosition.x + quad.mSize.x, quad.mPosition.y + quad.mSize.y},
          {quad.mColor.r, quad.mColor.g, quad.mColor.b,
           static_cast<uint8_t>(quad.mLifespan)},
          {0, 0}});

      vertices.push_back(
          SDL_Vertex{{quad.mPosition.x + quad.mSize.x, quad.mPosition.y},
                     {quad.mColor.r, quad.mColor.g, quad.mColor.b,
                      static_cast<uint8_t>(quad.mLifespan)},
                     {0, 0}});
      vertices.push_back(
          SDL_Vertex{{quad.mPosition.x, quad.mPosition.y},
                     {quad.mColor.r, quad.mColor.g, quad.mColor.b,
                      static_cast<uint8_t>(quad.mLifespan)},
                     {0, 0}});
      indices.push_back(offset);
      indices.push_back(offset + 1);
      indices.push_back(offset + 2);
      indices.push_back(offset + 2);
      indices.push_back(offset + 3);
      indices.push_back(offset);
    }
  }

  SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(),
                     indices.data(), indices.size());
}

void ParticleSystem::setOrigin(const Vec2<float> &origin)
{
  for (auto &p : this->mParticles)
    p.mOrigin = origin;
}

void ParticleSystem::moveParticles(Vec2<float> offset)
{
  for (auto &p : this->mParticles)
    p.mPosition += offset;
}

/* End */

}; // namespace HO
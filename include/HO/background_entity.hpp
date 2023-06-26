#pragma once

#include "HO/entity.hpp"
#include "HO/vec2.hpp"

struct SDL_Renderer;

namespace HO
{

class BackgroundEntity final : public Entity
{
public:
  BackgroundEntity();
  BackgroundEntity(Vec2<float> position, Vec2<float> size);
  void update(float delta);
  void render(SDL_Renderer *renderer);

protected:
};

}; // namespace HO
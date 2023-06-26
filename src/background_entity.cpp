#include "HO/background_entity.hpp"
#include "HO/config.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>

namespace HO
{

BackgroundEntity::BackgroundEntity()
    : Entity{Vec2<float>{0.0f, 0.0f}, Vec2<float>{64.0f, 64.0f}}
{
}

BackgroundEntity::BackgroundEntity(Vec2<float> position, Vec2<float> size)
    : Entity{position, size}
{
}

void BackgroundEntity::update(float delta)
{
  if (this->mPosition.y > 1080)
  {
    this->mPosition.y = -150.0f;
  }
  else
    this->move(Vec2<float>{0, delta * 0.3f});
}

void BackgroundEntity::render(SDL_Renderer *renderer)
{
  if (Config::debugView)
  {
    SDL_SetRenderDrawColor(renderer, 115, 0, 255, 255);
    SDL_RenderDrawRectF(renderer, &(this->mRenderRect));
  }
  else
  {
    SDL_RenderCopyF(renderer, this->mTexture.get(), nullptr,
                    &(this->mRenderRect));
  }
}

}; // namespace HO
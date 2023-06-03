#include "HO/texture.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <stdexcept>

namespace HO
{
Texture::Texture() : mTexture{nullptr} {}
Texture::Texture(SDL_Renderer *renderer, const std::string &name,
                 const std::string &path)
    : mName{name}, mTexture{createTextureOrThrow(renderer, path)}
{
}

void Texture::load(SDL_Renderer *renderer, const std::string &path)
{
  this->mTexture = std::unique_ptr<SDL_Texture, Deleters::SdlDeleter>(
      createTextureOrThrow(renderer, path));
}

SDL_Texture *Texture::get() const { return this->mTexture.get(); }

SDL_Texture *Texture::createTextureOrThrow(SDL_Renderer *renderer,
                                           const std::string &path)
{
  SDL_Texture *texture = nullptr;
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (!surface)
  {
    std::stringstream error;
    error << "Failed to load image: " << path << ", " << IMG_GetError()
          << std::endl;
    throw std::runtime_error(error.str());
  }
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return texture;
}
}; // namespace HO
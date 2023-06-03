#pragma once

#include "HO/deleters.hpp"
#include "HO/vec2.hpp"
#include <SDL.h>
#include <memory>
#include <string>

namespace HO
{
class Texture
{
public:
  Texture();
  Texture(SDL_Renderer *renderer, const std::string &name,
          const std::string &path);
  void load(SDL_Renderer *renderer, const std::string &path);
  SDL_Texture *get() const;

protected:
  struct TextureDeleter
  {
    void operator()(SDL_Texture *texture) { SDL_DestroyTexture(texture); }
  };
  SDL_Texture *createTextureOrThrow(SDL_Renderer *renderer,
                                    const std::string &path);
  std::string mName;
  std::unique_ptr<SDL_Texture, Deleters::SdlDeleter> mTexture;
};

}; // namespace HO
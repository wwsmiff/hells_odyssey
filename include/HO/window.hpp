#pragma once

#include "HO/deleters.hpp"
#include "HO/rgba.hpp"
#include <HO/vec2.hpp>
#include <SDL.h>
#include <functional>
#include <iostream>
#include <memory>

namespace HO
{
  class Window
  {
  public:
    Window(const std::string &title, const HO::Vec2<int32_t> &size,
           uint32_t flags);
    Vec2<int32_t> getSize();
    void clear(const HO::Rgba &color);
    void render();
    SDL_Renderer *getRenderer();

  protected:
    struct WindowDeleter
    {
      void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
      void operator()(SDL_Renderer *renderer) const
      {
        SDL_DestroyRenderer(renderer);
      }
    };
    SDL_Window *createWindowOrThrow(const std::string &title, uint32_t width,
                                    uint32_t height, uint32_t flags) const;
    SDL_Renderer *createRendererOrThrow(SDL_Window *window, int32_t index,
                                        uint32_t flags) const;
    std::unique_ptr<SDL_Window, Deleters::SdlDeleter> mWindow{};
    std::unique_ptr<SDL_Renderer, Deleters::SdlDeleter> mRenderer{};
    Vec2<int32_t> mSize{};
  };

}; // namespace HO
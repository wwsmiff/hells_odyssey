#include "HO/window.hpp"
#include <sstream>
#include <stdexcept>

namespace HO
{

Window::Window(const std::string &title, const HO::Vec2<int32_t> &size,
               uint32_t flags)
    : mSize{size}, mWindow{createWindowOrThrow(title, size.x, size.y, flags)},
      mRenderer(
          createRendererOrThrow(mWindow.get(), -1, SDL_RENDERER_ACCELERATED))
{
}

Vec2<int32_t> Window::getSize()
{
  SDL_GetWindowSize(this->mWindow.get(), &(this->mSize.x), &(this->mSize.y));
  return this->mSize;
}

void Window::clear(const HO::Rgba &color)
{
  SDL_SetRenderDrawColor(this->mRenderer.get(), color.r, color.g, color.b,
                         color.a);
  SDL_RenderClear(this->mRenderer.get());
}

void Window::render() { SDL_RenderPresent(this->mRenderer.get()); }

SDL_Renderer *Window::getRenderer() { return this->mRenderer.get(); }

SDL_Window *Window::createWindowOrThrow(const std::string &title,
                                        uint32_t width, uint32_t height,
                                        uint32_t flags) const
{
  SDL_Window *window =
      SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, width, height, flags);
  if (!window)
  {
    std::stringstream error;
    error << "Failed to create a window: " << SDL_GetError() << std::endl;
    throw std::runtime_error(error.str());
  }

  return window;
}

SDL_Renderer *Window::createRendererOrThrow(SDL_Window *window, int32_t index,
                                            uint32_t flags) const
{
  SDL_Renderer *renderer = SDL_CreateRenderer(window, index, flags);
  if (!renderer)
  {
    std::stringstream error;
    error << "Failed to create a renderer: " << SDL_GetError() << std::endl;
    throw std::runtime_error(error.str());
  }

  return renderer;
}

}; // namespace HO
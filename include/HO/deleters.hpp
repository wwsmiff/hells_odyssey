#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
typedef struct _SDL_Joystick SDL_Joystick;

namespace HO
{
namespace Deleters
{
struct SdlDeleter
{
  void operator()(SDL_Texture *texture) const;
  void operator()(SDL_Window *texture) const;
  void operator()(SDL_Renderer *texture) const;
  void operator()(SDL_Joystick *joystick) const;
};
}; // namespace Deleters
}; // namespace HO

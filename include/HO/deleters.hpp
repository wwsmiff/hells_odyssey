#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
typedef struct _SDL_Joystick SDL_Joystick;
typedef struct _SDL_GameController SDL_GameController;

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
  void operator()(SDL_GameController *gameController) const;
};
}; // namespace Deleters
}; // namespace HO

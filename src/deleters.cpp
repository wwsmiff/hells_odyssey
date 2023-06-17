#include "HO/deleters.hpp"
#include <SDL.h>

namespace HO
{
namespace Deleters
{
void SdlDeleter::operator()(SDL_Texture *texture) const
{
  SDL_DestroyTexture(texture);
}
void SdlDeleter::operator()(SDL_Window *window) const
{
  SDL_DestroyWindow(window);
}
void SdlDeleter::operator()(SDL_Renderer *renderer) const
{
  SDL_DestroyRenderer(renderer);
}
void SdlDeleter::operator()(SDL_Joystick *joystick) const
{
  SDL_JoystickClose(joystick);
}
void SdlDeleter::operator()(SDL_GameController *gameController) const
{

  SDL_GameControllerClose(gameController);
}

}; // namespace Deleters
}; // namespace HO

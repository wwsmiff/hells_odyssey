#include <HO/HO.hpp>
#include <SDL.h>
#include <cstdint>
#include <gvdi/gvdi.hpp>

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) !=
      0)
  {
    std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_DisplayMode primaryDisplay{};
  SDL_GetDesktopDisplayMode(0, &primaryDisplay);

  constexpr int32_t camera_width_v{800};
  // const int32_t camera_height_v{primaryDisplay.h};

  HO::Window mainWindow{"Hell's Odyssey",
                        HO::Vec2<int32_t>{primaryDisplay.w, primaryDisplay.h},
                        SDL_WINDOW_FULLSCREEN_DESKTOP};

  // HO::Window mainWindow{"Hell's Odyssey", HO::Vec2<int32_t>{800, 1024},
  //                       SDL_WINDOW_SHOWN};
  HO::Editor editor{HO::Vec2<float>{500, 600}};

  bool running{true};

  SDL_FRect gameBackground{0, 0, camera_width_v, 0};

  gameBackground.h = primaryDisplay.h;
  gameBackground.x =
      static_cast<int32_t>((primaryDisplay.w / 2) - (gameBackground.w / 2));

  HO::Vec2<uint32_t> gameBoundsHorizontal{
      static_cast<uint32_t>(gameBackground.x),
      static_cast<uint32_t>(gameBackground.x + gameBackground.w)};
  HO::Vec2<uint32_t> gameBoundsVertical{
      static_cast<uint32_t>(gameBackground.y),
      static_cast<uint32_t>(gameBackground.y + gameBackground.h)};

  // auto fromLeft = [=](float n) { return gameBoundsHorizontal.x + n; };
  // auto fromRight = [=](float n) { return gameBoundsHorizontal.y - n; };
  // auto fromTop = [=](float n) { return gameBoundsVertical.x + n; };
  auto fromBottom = [=](float n) { return gameBoundsVertical.y - n; };

  HO::Player player{
      HO::Vec2<float>{
          static_cast<float>(((gameBoundsHorizontal.x + (camera_width_v / 2)) -
                              (HO::Config::playerBlocksize / 2))),
          static_cast<float>(fromBottom(HO::Config::playerBlocksize + 50))},
      HO::Vec2<float>{HO::Config::playerBlocksize,
                      HO::Config::playerBlocksize}};
  player.setColor(HO::Rgba{0x2c'ee'2c'ff});

  player.setHitbox(
      HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
  player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                 HO::Config::playerBlocksize});

  player.setBounds(gameBoundsHorizontal, gameBoundsVertical);

  player.loadTexture(mainWindow.getRenderer(),
                     "../assets/sprites/player/ship.png");

  // gvdi::Instance instance{};
  // if constexpr (enable_debug_v)
  //   instance = gvdi::Instance{{500, 600}, "Tweaks"};

  HO::InputManager windowInput{};

  std::chrono::duration<float, std::milli> elapsedTime{};

  auto start{std::chrono::steady_clock::now()};

  windowInput.callbacks.emplace(SDL_QUIT, [&]() { running = false; });

  auto frame{0};
  auto fps{0};
  while (running)
  {
    auto current{std::chrono::steady_clock::now()};
    auto delta{std::chrono::duration<float, std::milli>(current - start)};

    windowInput.beginNewFrame();

    player.handleEvents(windowInput);

    if constexpr (enable_debug_v)
    {
      player.setHitbox(
          HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
      player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                     HO::Config::playerBlocksize});
    }

    player.update(delta.count());

    mainWindow.clear(HO::Rgba{0x00'00'00'ff});

    SDL_SetRenderDrawColor(mainWindow.getRenderer(), 16, 16, 16, 255);
    SDL_RenderFillRectF(mainWindow.getRenderer(), &gameBackground);

    player.render(mainWindow.getRenderer());

    mainWindow.render();

    editor.inspect();

    elapsedTime += delta;
    frame++;
    if (elapsedTime > std::chrono::seconds(1))
    {
      fps = frame;
      elapsedTime = std::chrono::duration<float, std::milli>{0};
      frame = 0;
    }

    start = current;
  }

  return EXIT_SUCCESS;
}
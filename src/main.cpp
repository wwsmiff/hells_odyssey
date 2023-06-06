#include <HO/HO.hpp>
#include <SDL.h>
#include <cstdint>
#include <gvdi/gvdi.hpp>

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
  {
    std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
    return 1;
  }

  gvdi::Instance instance{};
  if constexpr (enable_debug_v)
    instance = gvdi::Instance{{500, 600}, "Tweaks"};

  SDL_DisplayMode primaryDisplay{};
  SDL_GetDesktopDisplayMode(0, &primaryDisplay);

  constexpr uint32_t padding_v{16};
  constexpr int32_t camera_width_v{800};
  const int32_t camera_height_v{primaryDisplay.h};

  constexpr uint32_t fps_lock_v{60};
  constexpr uint32_t desired_fps_v{static_cast<uint32_t>(1000 / fps_lock_v)};

  HO::Window mainWindow{"Hell's Odyssey",
                        HO::Vec2<int32_t>{primaryDisplay.w, primaryDisplay.h},
                        SDL_WINDOW_FULLSCREEN_DESKTOP};
  bool running{true};

  SDL_Rect gameBackground{0, 0, camera_width_v, 0};

  gameBackground.h = primaryDisplay.h;
  gameBackground.x =
      static_cast<int32_t>((primaryDisplay.w / 2) - (gameBackground.w / 2));

  HO::Vec2<uint32_t> gameBoundsHorizontal{
      static_cast<uint32_t>(gameBackground.x),
      static_cast<uint32_t>(gameBackground.x + gameBackground.w)};
  HO::Vec2<uint32_t> gameBoundsVertical{
      static_cast<uint32_t>(gameBackground.y),
      static_cast<uint32_t>(gameBackground.y + gameBackground.h)};

  auto fromLeft = [=](float n) { return gameBoundsHorizontal.x + n; };
  auto fromRight = [=](float n) { return gameBoundsHorizontal.y - n; };
  auto fromTop = [=](float n) { return gameBoundsVertical.x + n; };
  auto fromBottom = [=](float n) { return gameBoundsVertical.y - n; };

  HO::Player player{HO::Vec2<float>{0.0f, 0.0f},
                    HO::Vec2<float>{HO::Config::playerBlocksize,
                                    HO::Config::playerBlocksize}};
  player.setPosition(HO::Vec2<float>{
      static_cast<float>(fromLeft(50)),
      static_cast<float>(fromBottom(HO::Config::playerBlocksize + 50))});
  player.setColor(HO::Rgba{0x2c'ee'2c'ff});

  player.setHitbox(
      HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
  player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                 HO::Config::playerBlocksize});

  player.setBounds(gameBoundsHorizontal, gameBoundsVertical);

  bool playerLeft{false}, playerRight{false}, playerUp{false},
      playerDown{false};

  player.loadTexture(mainWindow.getRenderer(),
                     "../assets/sprites/player_ship.png");
  HO::Vec2<int32_t> playerDirection{};

  auto start{SDL_GetTicks()};
  int32_t framesDrawn{0};
  int32_t frameCounter{0};

  SDL_Event windowEvent{};

  SDL_Joystick *primaryJoystick{};
  SDL_JoystickEventState(SDL_ENABLE);
  primaryJoystick = SDL_JoystickOpen(0);

  while (running)
  {
    auto current{SDL_GetTicks()};
    auto delta{current - start};

    while (SDL_PollEvent(&windowEvent))
    {
      if (windowEvent.type == SDL_QUIT)
        running = false;

      player.handleEvents(windowEvent);
    }

    if constexpr (enable_debug_v)
    {
      player.setHitbox(
          HO::Vec2<float>{HO::Config::playerHitbox, HO::Config::playerHitbox});
      player.setSize(HO::Vec2<float>{HO::Config::playerBlocksize,
                                     HO::Config::playerBlocksize});
    }

    player.update(static_cast<float>(delta));

    mainWindow.clear(HO::Rgba{0x00'00'00'ff});

    SDL_SetRenderDrawColor(mainWindow.getRenderer(), 16, 16, 16, 255);
    SDL_RenderFillRect(mainWindow.getRenderer(), &gameBackground);

    player.render(mainWindow.getRenderer());

    mainWindow.render();

    if constexpr (enable_debug_v)
    {
      gvdi::Frame frame{instance};
      ImGui::SetNextWindowPos({0, 0});
      ImGui::SetNextWindowSize({500, 600});
      ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoTitleBar);
      ImGui::Checkbox("Debug View", &HO::Config::debugView);

      ImGui::SliderFloat("Player velocity", &HO::Config::playerVelocity, 0.0f,
                         50.0f);

      int32_t playerBlocksizeInt{
          static_cast<int32_t>(HO::Config::playerBlocksize)};
      ImGui::SliderInt("Block size", &playerBlocksizeInt, 0, 1024);
      HO::Config::playerBlocksize = static_cast<uint32_t>(playerBlocksizeInt);

      int32_t playerHitboxInt{static_cast<int32_t>(HO::Config::playerHitbox)};
      ImGui::SliderInt("Hitbox size", &playerHitboxInt, 0, 1024);
      HO::Config::playerHitbox = static_cast<uint32_t>(playerHitboxInt);
      ImGui::End();
    }

    start = current;
  }

  return EXIT_SUCCESS;
}

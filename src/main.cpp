#include <HO/HO.hpp>
#include <SDL.h>
#include <cstdint>
#include <gvdi/gvdi.hpp>

constexpr bool enable_settings_v = true;

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
  {
    std::cout << "Failed to initialize SDL2, " << SDL_GetError() << std::endl;
    return 1;
  }

  gvdi::Instance instance{{500, 600}, "Tweaks"};

  SDL_DisplayMode primaryDisplay{};
  SDL_GetDesktopDisplayMode(0, &primaryDisplay);

  constexpr int32_t joystick_deadzone_v{8000};
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

  auto fromLeft = [=](int n) { return gameBoundsHorizontal.x + n; };
  auto fromRight = [=](int n) { return gameBoundsHorizontal.y - n; };
  auto fromTop = [=](int n) { return gameBoundsVertical.x + n; };
  auto fromBottom = [=](int n) { return gameBoundsVertical.y - n; };

  HO::Entity player{HO::Vec2<uint32_t>{0, 0},
                    HO::Vec2<uint32_t>{HO::Config::playerBlocksize,
                                       HO::Config::playerBlocksize}};
  player.setPosition(HO::Vec2<uint32_t>{
      fromLeft(50), fromBottom(HO::Config::playerBlocksize + 50)});
  player.setColor(HO::Rgba{0x2c'ee'2c'ff});

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
    player.setHitbox(
        HO::Vec2<uint32_t>{HO::Config::playerHitbox, HO::Config::playerHitbox});
    player.setSize(HO::Vec2<uint32_t>{HO::Config::playerBlocksize,
                                      HO::Config::playerBlocksize});
    while (SDL_PollEvent(&windowEvent))
    {
      if (windowEvent.type == SDL_QUIT)
        running = false;

      if (windowEvent.type == SDL_KEYDOWN)
      {
        switch (windowEvent.key.keysym.sym)
        {
        case SDLK_d:
          playerLeft = false;
          playerRight = true;
          playerDirection.x = 1;
          break;
        case SDLK_a:
          playerRight = false;
          playerLeft = true;
          playerDirection.x = -1;
          break;
        case SDLK_w:
          playerDown = false;
          playerUp = true;
          playerDirection.y = -1;
          break;
        case SDLK_s:
          playerUp = false;
          playerDown = true;
          playerDirection.y = 1;
          break;
        }
      }

      if (windowEvent.type == SDL_KEYUP)
      {
        switch (windowEvent.key.keysym.sym)
        {
        case SDLK_d:
          playerRight = false;
          playerDirection.x = 0;
          break;
        case SDLK_a:
          playerLeft = false;
          playerDirection.x = 0;
          break;
        case SDLK_w:
          playerUp = false;
          playerDirection.y = 0;
          break;
        case SDLK_s:
          playerDown = false;
          playerDirection.y = 0;
          break;
        }
      }

      /* Joystick support */
      if (windowEvent.type == SDL_JOYAXISMOTION)
      {
        if (windowEvent.jaxis.which == 0)
        {
          if (windowEvent.jaxis.axis == 0)
          {
            if (windowEvent.jaxis.value < -joystick_deadzone_v)
            {
              playerRight = false;
              playerLeft = true;
              playerDirection.x = -1;
            }
            else if (windowEvent.jaxis.value > joystick_deadzone_v)
            {
              playerLeft = false;
              playerRight = true;
              playerDirection.x = 1;
            }
            else
            {
              playerLeft = false;
              playerRight = false;
              playerDirection.x = 0;
            }
          }

          else if (windowEvent.jaxis.axis == 1)
          {
            if (windowEvent.jaxis.value < -joystick_deadzone_v)
            {
              playerDown = false;
              playerUp = true;
              playerDirection.y = -1;
            }
            else if (windowEvent.jaxis.value > joystick_deadzone_v)
            {
              playerUp = false;
              playerDown = true;
              playerDirection.y = 1;
            }
            else
            {
              playerUp = false;
              playerDown = false;
              playerDirection.y = 0;
            }
          }
        }
      }
    }

    HO::Vec2<float> playerDirectionNormalized = playerDirection.normalized();

    if (playerLeft)
    {
      if ((player.getPosition().x - padding_v) > gameBoundsHorizontal.x)
        player.move(
            HO::Vec2<int32_t>{static_cast<int32_t>(playerDirectionNormalized.x *
                                                   HO::Config::playerVelocity),
                              0});
    }
    if (playerRight)
    {
      if ((player.getPosition().x + player.getSize().x + padding_v) <
          gameBoundsHorizontal.y)
        player.move(
            HO::Vec2<int32_t>{static_cast<int32_t>(playerDirectionNormalized.x *
                                                   HO::Config::playerVelocity),
                              0});
    }
    if (playerUp)
    {
      if (player.getPosition().y > (gameBoundsVertical.x + padding_v))
        player.move(HO::Vec2<int32_t>{
            0, static_cast<int32_t>(playerDirectionNormalized.y *
                                    HO::Config::playerVelocity)});
    }
    if (playerDown)
    {
      if ((player.getPosition().y + player.getSize().y + padding_v) <
          gameBoundsVertical.y)
        player.move(HO::Vec2<int32_t>{
            0, static_cast<int32_t>(playerDirectionNormalized.y *
                                    HO::Config::playerVelocity)});
    }

    mainWindow.clear(HO::Rgba{0x00'00'00'ff});

    SDL_SetRenderDrawColor(mainWindow.getRenderer(), 16, 16, 16, 255);
    SDL_RenderFillRect(mainWindow.getRenderer(), &gameBackground);

    player.render(mainWindow.getRenderer());

    mainWindow.render();

    if constexpr (enable_settings_v)
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

    auto current{SDL_GetTicks()};
    auto delta{current - start};

    if (desired_fps_v > delta)
      SDL_Delay(desired_fps_v - delta);

    start = current;
  }

  return EXIT_SUCCESS;
}

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define MAP_SIZE 8

#define BLACK 0xFF000000
#define WHITE 0xFFFFFFFF
#define DARK_GRAY 0xFF333333
#define RED 0xFF0000FF
#define GREEN 0xFF00FF00
#define BLUE 0xFFFF0000

typedef uint32_t u32;
typedef uint16_t u16;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static SDL_FRect screen;

// player stuff
static float player_x = MAP_SIZE / 2.f, player_y = MAP_SIZE / 2.f;
static float player_angle = 0.f, fov = 120.0f;

// clang-format off
static u32 pixels[WINDOW_WIDTH * WINDOW_HEIGHT];
static const u32 map[MAP_SIZE * MAP_SIZE] = {
    1, 1, 1, 1, 1, 1, 1, 1, \
    1, 0, 0, 0, 0, 0, 0, 1, \
    1, 1, 0, 1, 1, 1, 0, 1, \
    1, 0, 0, 0, 0, 0, 0, 1, \
    1, 0, 2, 2, 2, 0, 0, 1, \
    1, 0, 0, 0, 0, 0, 0, 1, \
    1, 0, 0, 0, 0, 0, 0, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, \
};
// clang-format on

bool show_map = true;

int init();
void cleanup();
void createTexture();
void drawLevelTopDown(const u32 *map, u32 length);

int main() {
  bool done = false;

  printf("STARTING DOOM STYLE RENDERER\n");
  if (init()) {
    printf("Failed to init sdl\nExiting now...\n");
    return 1;
  }

  screen.x = 0.0f;
  screen.y = 0.0f;
  screen.w = (float)WINDOW_WIDTH;
  screen.h = (float)WINDOW_HEIGHT;

  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_M) {
          show_map = !show_map;
        }
      }
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }
    SDL_SetRenderDrawColorFloat(renderer, 0.5, 0.5, 0.5,
                                SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    // draw stuff
    if (show_map) {
      drawLevelTopDown(map, MAP_SIZE);
    } else {
      createTexture();
    }

    // update screen
    SDL_RenderTexture(renderer, texture, NULL, &screen);
    SDL_RenderPresent(renderer);
  }

  cleanup();
  return 0;
}

void drawLevelTopDown(const u32 *map, u32 length) {
  u32 section_width = WINDOW_WIDTH / length;
  u32 section_height = WINDOW_HEIGHT / length;

  for (u32 i = 0; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) {
    u32 mx = (i % WINDOW_WIDTH) / section_width;
    u32 my = (i / WINDOW_WIDTH) / section_height;

    u32 color = map[length * my + mx];

    if (color == 0) {
      pixels[i] = BLACK;
    } else if (color == 1) {
      pixels[i] = RED;
    } else if (color == 2) {
      pixels[i] = GREEN;
    } else if (color == 3) {
      pixels[i] = BLUE;
    }
  }
  SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
}

void createTexture() {
  const u32 cells_width = 80;
  for (u32 i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
    u32 x = i % WINDOW_WIDTH;
    u32 y = i / WINDOW_WIDTH;

    if (((x / cells_width) % 2 == 0 && (y / cells_width) % 2 == 0) ||
        ((x / cells_width) % 2 == 1 && (y / cells_width) % 2 == 1)) {
      // dark
      pixels[i] = 0xFF72d713;
    } else {
      // light
      pixels[i] = 0xFF5BB065;
    }
  }
  SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
}

int init() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to init SDL subsystem: %s",
                 SDL_GetError());
    return 1;
  }
  if (!SDL_CreateWindowAndRenderer("Doom", WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE,
                                   &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Failed to create window/.renderer: %s", SDL_GetError());
    return 1;
  }
  SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  memset(pixels, DARK_GRAY, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(u32));
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                              SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH,
                              WINDOW_HEIGHT);
  SDL_UpdateTexture(texture, NULL, pixels, WINDOW_WIDTH * 4);
  return 0;
}

void cleanup() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

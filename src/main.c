#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include <SDL3/SDL_render.h>
#include <stdio.h>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

int main() {
  bool done = false;
  SDL_Window *window;
  SDL_Renderer *renderer;

  // ------ INIT ----------
  printf("STARTING DOOM STYLE RENDERER\n");
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
  // -----------------------

  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    const double now = ((double)SDL_GetTicks()) / 1000.0;
    float red = (float)(0.5 + 0.5 * SDL_sin(now));
    float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));

    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  // cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

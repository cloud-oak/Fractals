#include "fractals.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

int main()
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool quit = false;

  init();
  
  int frame = 0;
  while (!quit)
  {
    while (SDL_PollEvent(&event) > 0) {
      if (event.type == SDL_QUIT)
        quit = true;
      if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == 1) {
        float nWidthHalf = (MAXX - MINX) / 4;
        float nHeightHalf = (MAXY - MINY) / 4;
        float nCenterX = double(event.button.x) / WIDTH  * (MAXX-MINX) + MINX;
        float nCenterY = double(event.button.y) / HEIGHT * (MAXY-MINY) + MINY;
        MINX = nCenterX - nWidthHalf;
        MAXX = nCenterX + nWidthHalf;
        MINY = nCenterY - nHeightHalf;
        MAXY = nCenterY + nHeightHalf;

        MAX_ITERATIONS += 5;

        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    calculate_frame();

    for(int x = 0; x < WIDTH; x++) {
      for(int y = 0; y < HEIGHT; y++) {
        auto i = iters[x][y];
        SDL_SetRenderDrawColor(renderer, R[i], G[i], B[i], 255);
        SDL_RenderDrawPoint(renderer, x, y);
      }
    }

    // SDL_RenderPresent(renderer);
    if (frame < FRAMES) {
      const Uint32 format = SDL_PIXELFORMAT_ARGB8888;
      SDL_Surface* pScreenShot = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, format);

      if(pScreenShot)
      {
        SDL_RenderReadPixels(renderer, NULL, format, pScreenShot->pixels, pScreenShot->pitch);

        ostringstream str;
        str << "/tmp/" << setfill('0') << setw(4) << frame << ".bmp";
        SDL_SaveBMP(pScreenShot, str.str().c_str());
        ++frame;

        SDL_FreeSurface(pScreenShot);
      } 
    }
    else
    {
      quit = true;
    }
  }
}

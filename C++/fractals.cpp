#include "SDL.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#include <thread>
#include <iostream>
#include <complex>

using namespace std;

int MAX_ITERATIONS = 50;

double MINX = -2;
double MAXX =  1;

double MINY = -1;
double MAXY =  1;

#define WIDTH 1200
#define HEIGHT 800

#define NUM_THREADS 8


double iters[WIDTH][HEIGHT];

vector<thread> threads(NUM_THREADS);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

void calcRange(int minX, int maxX)
{
	SDL_Rect myrect;
	for(int x = minX; x<maxX; x++) {
		for(int y = 0; y<HEIGHT; y ++) {
			complex<double> c = {double(x) / WIDTH  * (MAXX-MINX) + MINX,
								 double(y) / HEIGHT * (MAXY-MINY) + MINY};
			complex<double> z = c;
			for(iters[x][y] = 0; iters[x][y] < MAX_ITERATIONS && abs(z) < 4; iters[x][y]++) {
				z = z*z + c;
			}
		}
	}

}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool quit = false;
	bool invalid = true;

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

					invalid = true;
				break;
			}
		}

		if(invalid)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			
			for(int i = 0; i<NUM_THREADS; i++)
			{
				threads[i] = thread(calcRange,
				 	i   *  WIDTH / (NUM_THREADS),
					(i+1)* WIDTH / (NUM_THREADS));
			}
			for(int i = 0; i<NUM_THREADS; i++)
			{
				threads[i].join();
			}

			for(int x = 0; x < WIDTH; x++) {
				for(int y = 0; y < HEIGHT; y++) {
					SDL_SetRenderDrawColor(renderer,
						255 * iters[x][y] / MAX_ITERATIONS,
						0,
						255 - (255 / MAX_ITERATIONS)*iters[x][y],
						255);
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
					
			SDL_RenderPresent(renderer);

			invalid = false;
		}
	}
}

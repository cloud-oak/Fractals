#include "SDL.h"
#include <vector>
#include <complex>
#include <thread>
#include <cmath>
#include <iostream>

using namespace std;

#define WIDTH 800
#define HEIGHT 800

#define NUM_THREADS 8
#define FRAMES 400

int MAX_ITERATIONS = 8;

int iters[WIDTH][HEIGHT];

vector<thread> threads(NUM_THREADS);

double MINX = -2;
double MAXX =  2;

double MINY = -2;
double MAXY =  2;

double phi = 0;
const double dphi = 2 * M_PI / double(FRAMES);
complex<double> e;

vector<int> R;
vector<int> G;
vector<int> B;

double r(double i)
{
  return 4 * i * (1 - i);
}
double g(double i)
{
  return 4 * i * (1 - i);
}
double b(double i)
{
  return 4 * i * (1 - i);
}
void init()
{
  R = vector<int>(MAX_ITERATIONS + 1);
  G = vector<int>(MAX_ITERATIONS + 1);
  B = vector<int>(MAX_ITERATIONS + 1);

  for(int i = 0; i < MAX_ITERATIONS; ++i) {
    double t = double(MAX_ITERATIONS - i) / double(MAX_ITERATIONS);
    t = t * t;
    R[i] = Uint8(255 * r(t));
    G[i] = Uint8(255 * g(t));
    B[i] = Uint8(255 * b(t));
  }
}

void calcRange(int minX, int maxX)
{
  for(int x = minX; x < maxX; x++) {
    for(int y = 0; y < HEIGHT; y ++) {
      complex<double> c = {double(x) / WIDTH  * (MAXX-MINX) + MINX,
        double(y) / HEIGHT * (MAXY-MINY) + MINY};
      complex<double> z = c;

      iters[x][y] = 0;
      for(; iters[x][y] < MAX_ITERATIONS && abs(z) < 3; iters[x][y]++) {
        // z = conj(z * z * e) + c - 0.2 * e;
        z = pow(1.0 / (0.3 * pow(z, 5) - e), 2) + c;
      }
    }
  }
}

void calculate_frame()
{
  phi += dphi;
  e = exp(1i * phi);

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
}

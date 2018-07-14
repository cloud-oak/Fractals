#include "fractals.hpp"
#include <chrono>
#include <iostream>
#include <limits>

#include <stack>

using namespace std;
using namespace std::chrono;

stack<steady_clock::time_point> tictoc_stack;

void tic() {
  tictoc_stack.push(steady_clock::now());
}

double toc() {
  auto end = steady_clock::now();
  double ms = duration_cast<duration<double, milli>>(end - tictoc_stack.top()).count();
  tictoc_stack.pop();
  return ms;
}


#define ITERS 10

int main() {
  vector<double> times(ITERS);

  for ( int i = 0; i < ITERS; ++i ) {
    tic();
    calculate_frame();
    times[i] = toc();
  }

  double sum = 0, max = 0, min = numeric_limits<double>::max();
  for (auto t : times) {
    sum += t;
    if (t > max)
      max = t;
    if (t < min)
      min = t;
  }
  sum /= ITERS;

  cout << "Average over " << ITERS << " runs: " << sum << "ms" << endl;
  cout << "Min: " << min << "ms Max: " << max << "ms" << endl;
}

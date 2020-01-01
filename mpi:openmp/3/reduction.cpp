#include <omp.h>
#include <iostream>
#include <cstdlib>

const double dx = 1e-9;
const int N = 1e9;

double f(double x)
{
  return 1 / (1 + x * x);
}

int main(int argc, char *argv[])
{

  double begin, end, total;

  int N = std::atoi(argv[1]);

  for (int i = 1; i < N; ++i)
  {
    omp_set_num_threads(std::atoi(argv[1]));
    begin = omp_get_wtime();
    double global_sum = 0;
#pragma omp parallel shared(global_sum)
    {
      double local_sum = 0;
#pragma omp for
      for (int i = 0; i < N; ++i)
      {
        local_sum += dx * (f(i * dx) + f((i + 1) * dx)) / 2;
      }
#pragma omp critical
      {
        global_sum += local_sum;
      }
    }

    end = omp_get_wtime();
    std::cout << end - begin << '\n';
  }

  return 0;
}
// Greg Attra
// 03/05/2021

#include <stdio.h>
#include <math.h>
#include <chrono>


int main(int argc, char** argv)
{
    auto start_pow = std::chrono::high_resolution_clock::now();
    pow(15, 2);
    auto stop_pow = std::chrono::high_resolution_clock::now();
    auto duration_pow = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_pow - start_pow);

    auto start_mult = std::chrono::high_resolution_clock::now();
    int i = 15 * 15;
    auto stop_mult = std::chrono::high_resolution_clock::now();
    auto duration_mult = std::chrono::duration_cast<std::chrono::nanoseconds>(stop_mult - stop_mult);

    printf("Time for pow(): %ld\n", duration_pow.count());
    printf("Time for mutliplication: %ld\n", duration_mult.count());

    return 0;
}
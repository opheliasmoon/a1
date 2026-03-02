#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <omp.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./saxpy N\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);

    float alpha = 2.5f;

    std::vector<float> x(N), y(N), y_parallel(N);

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (size_t i = 0; i < N; i++) {
        x[i] = dist(gen);
        y[i] = dist(gen);
        y_parallel[i] = y[i];
    }

    // SERIAL
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        y[i] = alpha * x[i] + y[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    double serial_time =
        std::chrono::duration<double>(end - start).count();

    // PARALLEL
    start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for
    for (size_t i = 0; i < N; i++) {
        y_parallel[i] = alpha * x[i] + y_parallel[i];
    }

    end = std::chrono::high_resolution_clock::now();
    double parallel_time =
        std::chrono::duration<double>(end - start).count();

    // Check error
    double error = 0.0;
    for (size_t i = 0; i < N; i++) {
        error += std::abs(y[i] - y_parallel[i]);
    }

    std::cout << "N = " << N << "\n";
    std::cout << "Serial time: " << serial_time << " sec\n";
    std::cout << "Parallel time: " << parallel_time << " sec\n";
    std::cout << "Total error: " << error << "\n";

    return 0;
}
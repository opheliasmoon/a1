#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./histogram_gather N\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);
    const unsigned int NUM_BINS = 10;

    std::vector<unsigned int> data(N);
    std::vector<unsigned int> hist_serial(NUM_BINS, 0);
    std::vector<unsigned int> hist_parallel(NUM_BINS, 0);

    std::mt19937 gen(42);
    std::uniform_int_distribution<unsigned int> dist(1, 100);

    for (size_t i = 0; i < N; i++)
        data[i] = dist(gen);

    // SERIAL HISTOGRAM
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        unsigned int bin = data[i] % NUM_BINS;
        hist_serial[bin]++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double serial_time =
        std::chrono::duration<double>(end - start).count();

    // PARALLEL HISTOGRAM
    start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        std::vector<unsigned int> local_hist(NUM_BINS, 0);

        #pragma omp for
        for (size_t i = 0; i < N; i++) {
            unsigned int bin = data[i] % NUM_BINS;
            local_hist[bin]++;
        }

        #pragma omp critical
        {
            for (unsigned int b = 0; b < NUM_BINS; b++)
                hist_parallel[b] += local_hist[b];
        }
    }

    end = std::chrono::high_resolution_clock::now();
    double parallel_time =
        std::chrono::duration<double>(end - start).count();

    // Check error
    unsigned int error = 0;
    for (unsigned int i = 0; i < NUM_BINS; i++)
        error += std::abs((int)hist_serial[i] - (int)hist_parallel[i]);

    std::cout << "N = " << N << "\n";
    std::cout << "Serial time: " << serial_time << " sec\n";
    std::cout << "Parallel time: " << parallel_time << " sec\n";
    std::cout << "Histogram error: " << error << "\n";

    return 0;
}
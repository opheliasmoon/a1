#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./montecarlo_pi N\n";
        return 1;
    }

    size_t N = std::stoull(argv[1]);

    // SERIAL
    size_t inside = 0;

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; i++) {
        float x = dist(gen);
        float y = dist(gen);
        if (x*x + y*y <= 1.0f)
            inside++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double serial_time =
        std::chrono::duration<double>(end - start).count();

    double pi_serial = 4.0 * inside / N;

    // PARALLEL
    size_t inside_parallel = 0;

    start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        std::mt19937 gen_local(42 + omp_get_thread_num());
        std::uniform_real_distribution<float> dist_local(0.0f, 1.0f);

        size_t local_count = 0;

        #pragma omp for
        for (size_t i = 0; i < N; i++) {
            float x = dist_local(gen_local);
            float y = dist_local(gen_local);
            if (x*x + y*y <= 1.0f)
                local_count++;
        }

        #pragma omp atomic
        inside_parallel += local_count;
    }

    end = std::chrono::high_resolution_clock::now();
    double parallel_time =
        std::chrono::duration<double>(end - start).count();

    double pi_parallel = 4.0 * inside_parallel / N;

    std::cout << "N = " << N << "\n";
    std::cout << "Serial pi: " << pi_serial << "\n";
    std::cout << "Parallel pi: " << pi_parallel << "\n";
    std::cout << "Serial time: " << serial_time << " sec\n";
    std::cout << "Parallel time: " << parallel_time << " sec\n";
    std::cout << "Error: " << std::abs(pi_serial - pi_parallel) << "\n";

    return 0;
}
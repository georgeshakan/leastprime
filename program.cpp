// program.cpp
#include "program.hpp"
#include <unordered_set>
#include <cmath>
#include <cassert>
#include <fstream>
#include <chrono> 
#include <cstdlib>

// Function to compute the greatest common divisor (GCD)
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to generate all prime numbers up to N using the Sieve of Eratosthenes
std::vector<int> generate_primes(int N) {
    std::vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    int sqrt_N = static_cast<int>(std::sqrt(N));
    for (int i = 2; i <= sqrt_N; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= N; j += i)
                is_prime[j] = false;
        }
    }
    std::vector<int> primes;
    for (int i = 2; i <= N; ++i) {
        if (is_prime[i])
            primes.push_back(i);
    }
    return primes;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " Number_of_Modulus" << std::endl;
        return 1;
    }

    int N = 100'000'0000;
    int N_modulus = std::atoi(argv[1]);

    // Generate primes up to N
    std::vector<int> primes = generate_primes(N);
    std::unordered_set<int> primes_set(primes.begin(), primes.end());

    // Initialize records for moduli from 2 to N_modulus - 1
    std::vector<int> records(N_modulus - 2);

    // Time measurement with std::chrono
    auto start = std::chrono::high_resolution_clock::now();

    // Error flag
    bool need_more_primes = false;
    int tmp, ell,ap,max_prime, modulus;
    // Parallelize the outer loop over modulus using OpenMP
    #pragma omp parallel for schedule(dynamic) private(modulus, max_prime, ap, ell, tmp) shared(N_modulus, primes_set, primes, records, need_more_primes)
    for (int modulus = 2; modulus < N_modulus; ++modulus) {
        int max_prime = -1;
        for (int ap = 1; ap < modulus; ++ap) {
            if (gcd(ap, modulus) == 1) {
                ell = 0;
                while (true) {
                    tmp = ap + ell * modulus;
                    if (tmp > primes.back()) {
                        // Set the flag atomically
                        #pragma omp atomic write
                        need_more_primes = true;
                        break; // Exit the loop
                    }
                    if (primes_set.count(tmp)) {
                        max_prime = std::max(max_prime, tmp);
                        break; // Found the smallest prime for this ap
                    }
                    ++ell;
                }
            }
        }
        records[modulus - 2] = max_prime;
    }

    // Check if we need more primes
    if (need_more_primes) {
        std::cerr << "We need more primes. Increase N." << std::endl;
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    // Calculate and print the approximate size of records in MB
    std::cout << "Size of records: " << records.capacity() * sizeof(int) / 1e6 << " MB" << std::endl;

    // Write records to a file
    std::ofstream outfile("records.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }
    for (int modulus = 2; modulus < N_modulus; ++modulus) {
        outfile << modulus << ":" << records[modulus - 2] << "\n";
    }
    outfile.close();
    return 0;
}

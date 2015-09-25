//
// Created by William Lees on 9/21/15.
//

#include <iostream>
#include <chrono>
#include <cmath>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <thread>

#define     MAX_FIB_CALCULABLE      1500
#define     SHARED_MEMORY_NAME      "fib_shared_mem"

double              *fib;
short               maxIndex;
double              phi;
double              phi_not;
double              one_over_root_five;
int                 shared_memory;

double calculateFibIndex(short i);
void calculateFibUpToIndex(short index);
void toString();

int main() {
    // Initialize the variables
    phi = (1.0+sqrt(5.0))/2.0;
    phi_not = (1.0-sqrt(5.0))/2.0;
    one_over_root_five = 1.0/sqrt(5.0);

    // Create the shared memory
    shared_memory = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0777);
    ftruncate(shared_memory, MAX_FIB_CALCULABLE*sizeof(double));
    // write(shared_memory, fib, MAX_FIB_CALCULABLE*sizeof(double));

    // Release the current fib array and point to the shared memory
    delete [] fib;
    fib = (double*)mmap(0, MAX_FIB_CALCULABLE*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);

    // List of fib number memoization
    short index;

    std::cout << "Calculate Fibonacci index: ";
    std::cin >> index;

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    calculateFibUpToIndex(index+1);

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

    toString();

    std::cout << "\nElapsed Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime-startTime).count()/1000000. << " ms";

    // Release the resources
    close(shared_memory);
    munlock(fib, MAX_FIB_CALCULABLE*sizeof(double));
    shm_unlink(SHARED_MEMORY_NAME);

    return 0;
}



void calculateFibUpToIndex(short index) {
    if (index > MAX_FIB_CALCULABLE) {
        std::cout << "Attempting to calculate up to index greater than max" << std::endl;
        return;
    }

    maxIndex = index;

    for (short i = 0; i < index; ++i) {
        pid_t  pid;
        while((pid = fork()) < 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            pid = fork();
        }

        std::cout << i << std::endl;

        if (pid == 0) {
            // --child
            fib = (double*)mmap(nullptr, MAX_FIB_CALCULABLE*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);

            // Calculate the #
            fib[i] = calculateFibIndex(i);


            // Release the shared memory link
            close(shared_memory);
            exit(0);

        } else if (pid > 0) {
            // Parent
            continue;
        } else {
            std::cerr << "Failed fork()" << std::endl;
            return;
        }
    }
}

double calculateFibIndex(short i) {
    // Calculate Fib
    return one_over_root_five * pow(phi, i) - one_over_root_five * pow(phi_not, i);
}

void toString() {
    for (short i = 0; i < maxIndex; ++i) {
        std::cout << "Fib(" << std::to_string(i) << ")=" << std::to_string(fib[i]) << "\n";
    }
}



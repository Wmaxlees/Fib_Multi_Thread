#include <iostream>
#include <chrono>
#include <cmath>

#define     MAX_FIB_CALCULABLE      1500

double              *fib;
pthread_t           *threads;
short               maxIndex;
double              phi;
double              phi_not;
double              one_over_root_five;

void *calculateFibIndex(void *i);
void calculateFibUpToIndex(short index);
void toString();

int main() {
    // Initialize the variables
    fib = new double[MAX_FIB_CALCULABLE];
    threads = new pthread_t[MAX_FIB_CALCULABLE];
    phi = (1.0+sqrt(5.0))/2.0;
    phi_not = (1.0-sqrt(5.0))/2.0;
    one_over_root_five = 1.0/sqrt(5.0);


    // List of fib number memoization
    short index;

    std::cout << "Calculate Fibonacci index: ";
    std::cin >> index;

    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

    calculateFibUpToIndex(index+1);

    std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

    toString();

    std::cout << "\nElapsed Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(endTime-startTime).count()/1000000. << " ms";

    return 0;
}

void calculateFibUpToIndex(short index) {
    if (index > MAX_FIB_CALCULABLE) {
        std::cout << "Attempting to calculate up to index greater than max" << std::endl;
        return;
    }

    maxIndex = index;

    for (short i = 0; i < index; ++i) {
        short temp = i;
        pthread_create(&(threads[i]), NULL, calculateFibIndex, (void*)&temp);
    }

    for (short i = 0; i < index; ++i) {
        pthread_join(threads[i], NULL);
    }
}

void *calculateFibIndex(void *i) {
    // Calculate Fib
    fib[*(short *)i] = one_over_root_five * pow(phi, *(short *)i) - one_over_root_five * pow(phi_not, *(short *)i);

    return nullptr;
}

void toString() {
    for (short i = 0; i < maxIndex; ++i) {
        std::cout << "Fib(" << std::to_string(i) << ")=" << std::to_string(fib[i]) << "\n";
    }
}


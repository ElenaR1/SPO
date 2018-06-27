//
//  main.cpp
//  SlowPiCalculator
//
//  Created by Мирослав Николов on 9.06.18 г..
//  Copyright © 2018 г. Мирослав Николов. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <thread>
#include <vector>
#include <cmath>
#include <chrono>
#include "CLParser.h"
#include <unordered_map>
#include <fstream>
//#include <unistd.h>
//using namespace std;
//std::unordered_map<int, long double> resultsOfThreads;
long double *resultsOfThreads;
std::string fileName;
int threadsCount;
int iterationsCount;
int testCount;
int quiet;

__int128_t factoriel(int n) {
    __int128_t val = 1;
    for (int i = 1; i <= n; i++) {
        val *= i;
    }
    return val;
}

long double subFunction(int n) {
    __int128_t onePrefix = 1;
    
    if (n%2==1) {
        onePrefix = -1;
    }
    
    __int128_t n4Fact = factoriel(4*n);
    __int128_t firstPartUp = onePrefix*n4Fact;
    
    __int128_t nFact = factoriel(n);
    __int128_t fourDegreeN = pow(4,n);
    __int128_t firstPartDown = pow(fourDegreeN*nFact,4);
    long double firstPart = (long double)((long double)firstPartUp/(long double)firstPartDown);
    
    __int128_t secondPartUp = 1123 + 21460*n;
    __int128_t secondPartDown = pow((__int128_t)882, (__int128_t)(2*n));
    long double sedondPart = ((long double)secondPartUp)/((long double)secondPartDown);
    
    long double result = (long double)(((long double)firstPart)*((long double)sedondPart));
    return result;
}

//long double sumOfSubFunction(int to) {
//    long double result = 0;
//    for (int i = 0; i <= to; i++) {
//        result += subFunction(i);
//    }
//    return result;
//}

void threadFunction(int threadIndex, int size) {
//    for (int i = from; i < to; i++) {
//        resultsOfThreads[i] = subFunction(i);
//    }
    
    for (int i = threadIndex*size; i < iterationsCount ; i+=(threadsCount*size)) {
        for (int j = i; j < i + size && j < iterationsCount; j++) {
            resultsOfThreads[j] = subFunction(j);
        }
    }
}
void threadFunctionDecorator(int threadIndex, int size) {
    printf("Thread-%d started \n",threadIndex);
    auto start = std::chrono::steady_clock::now();
    threadFunction(threadIndex, size);
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    printf("Thread-%d stoped \n",threadIndex);
    printf("Thread-%d execution time was: %f \n",threadIndex, std::chrono::duration <double, std::milli> (diff).count());
}
void InitMap() {
    delete [] resultsOfThreads;
    resultsOfThreads = new long double[iterationsCount];
    for (int i = 0; i < iterationsCount; i++) {
        resultsOfThreads[i] = 0;
    }
}

long double InitThreads() {
    auto start = std::chrono::steady_clock::now();
    InitMap();
    long double result = 0;
    int size = 5;
    std::vector<std::thread> threads;
    for (int i=0; i<threadsCount; ++i) {
        if(quiet == -1) {
            threads.push_back(std::thread(threadFunctionDecorator, i, size));
        }
        else  threads.push_back(std::thread(threadFunction, i, size));
    }
    
//    for (int i=0; i<threadsCount + 1; i++) {
//        threads.push_back(std::thread(threadFunction, i*size, i*size + size));
//    }
    
    
    for(auto& thread: threads) {
        thread.join();
    }
    
    for (int i = 0; i < iterationsCount; i++) {
        result += resultsOfThreads[i];
    }
    
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    printf("Total execution time for current run: %f \n", std::chrono::duration <double, std::milli> (diff).count());
    
    result = (long double)((long double)(4*882)/(long double)result);
    return result;
}

void benchmark(int threads , int iterations, int _testCount) {
    
    threadsCount = threads;
    iterationsCount = iterations;
    testCount = _testCount;
    
    
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < testCount; i++) {
        InitMap();
        double result = InitThreads();
    }
    //    std::cout<<std::setprecision(90)<<result<<std::endl;
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "threads: "<<threadsCount << "   iterations: "<<iterationsCount << "   ";
    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
}

void parseArgs(int argc, const char * argv[]) {
    CLParser cmd_line(argc,argv,true);
    
    iterationsCount = cmd_line.get_arg_int("-p");
    threadsCount = cmd_line.get_arg_int("-t");
    fileName = cmd_line.get_arg("-o");
    quiet = cmd_line.get_arg_int("-q");
    
    if(iterationsCount == -1) iterationsCount = 1;
    if(threadsCount == -1) threadsCount = 1;
}

void saveOutputInFile(long double num)
{
    if(fileName != "") {
        std::ofstream myfile;
        myfile.open (fileName);
        myfile << std::setprecision(100) << num << "\n";
        myfile.close();
    }
}

int main(int argc, const char * argv[]) {
    threadsCount = 2;
    iterationsCount = 2;
    parseArgs(argc,argv);
    
    long double result = InitThreads();
    saveOutputInFile(result);
//    std::cout<<"Result: "<<std::setprecision(100)<<result<<std::endl;
    
//    benchmark(13,1110,100);
//    benchmark(12,1110,100);
//    benchmark(11,1110,100);
//    benchmark(10,1110,100);
//    benchmark(9,1110,100);
//    benchmark(8,1110,500);
//    benchmark(7,1110,500);
//    benchmark(6,1110,500);
    
//    benchmark(300,1110,500);
//    benchmark(100,1110,500);
//    benchmark(5,1110,500);
//    benchmark(4,1110,500);
//    benchmark(3,1110,500);
//    benchmark(2,1110,500);
//    benchmark(1,1110,500);
    
    
    return 0;
}

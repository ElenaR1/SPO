
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#include <thread>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <stdlib.h>




class CLParser
{
public:

	CLParser(int argc_, const char * argv_[], bool switches_on_ = false);
	~CLParser() {}

	std::string get_arg(int i);
	std::string get_arg(std::string s);
	int get_arg_int(std::string s);

private:

	int argc;
	std::vector<std::string> argv;

	bool switches_on;
	std::map<std::string, std::string> switch_map;
};

CLParser::CLParser(int argc_, const char * argv_[], bool switches_on_)
{
	argc = argc_;
	argv.resize(argc);
	copy(argv_, argv_ + argc, argv.begin());
	switches_on = switches_on_;


	if (switches_on)
	{
		std::vector<std::string>::iterator it1, it2;
		it1 = argv.begin();
		it2 = it1 + 1;

		while (true)
		{
			if (it1 == argv.end()) break;
			if (it2 == argv.end()) {
				switch_map[*it1] = "";
				break;
			}

			if ((*it1)[0] == '-')
				switch_map[*it1] = *(it2);

			it1++;
			it2++;
		}
	}
}

std::string CLParser::get_arg(int i)
{
	if (i >= 0 && i<argc)
		return argv[i];

	return "";
}

std::string CLParser::get_arg(std::string s)
{
	if (!switches_on) return "";

	if (switch_map.find(s) != switch_map.end())
		return switch_map[s];

	return "";
}

int CLParser::get_arg_int(std::string s)
{
	if (!switches_on) return 0;

	if (switch_map.find(s) != switch_map.end()) {
		if (switch_map[s].size() == 0) return 0;
		return stoi(switch_map[s]);
	}


	return -1;
}



long double *resultsOfThreads;
std::string fileName;
int threadsCount;
int iterationsCount;
int testCount;
int quiet;

long long factoriel(int n) {
	long long val = 1;
	for (int i = 1; i <= n; i++) {
		val *= i;
	}
	return val;
}

long double subFunction(int n) {
	long long onePrefix = 1;

	if (n % 2 == 1) {
		onePrefix = -1;
	}

	long long n4Fact = factoriel(4 * n);
	long long firstPartUp = onePrefix * n4Fact;

	long long nFact = factoriel(n);
	long long fourDegreeN = pow(4, n);
	long long firstPartDown = pow(fourDegreeN*nFact, 4);
	long double firstPart = (long double)((long double)firstPartUp / (long double)firstPartDown);

	long long secondPartUp = 1123 + 21460 * n;
	long long secondPartDown = pow((long long)882, (long long)(2 * n));
	long double sedondPart = ((long double)secondPartUp) / ((long double)secondPartDown);

	long double result = (long double)(((long double)firstPart)*((long double)sedondPart));
	return result;
}



void threadFunction(int threadIndex, int size) {


	for (int i = threadIndex * size; i < iterationsCount; i += (threadsCount*size)) {
		for (int j = i; j < i + size && j < iterationsCount; j++) {
			resultsOfThreads[j] = subFunction(j);
		}
	}
}
void threadFunctionDecorator(int threadIndex, int size) {
	printf("Thread-%d started \n", threadIndex);
	auto start = std::chrono::steady_clock::now();
	threadFunction(threadIndex, size);
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	printf("Thread-%d stoped \n", threadIndex);
	printf("Thread-%d execution time was: %f \n", threadIndex, std::chrono::duration <double, std::milli>(diff).count());
}
void InitMap() {
	delete[] resultsOfThreads;
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
	for (int i = 0; i<threadsCount; ++i) {
		if (quiet == -1) {
			threads.push_back(std::thread(threadFunctionDecorator, i, size));
		}
		else  threads.push_back(std::thread(threadFunction, i, size));
	}



	for (auto& thread : threads) {
		thread.join();
	}

	for (int i = 0; i < iterationsCount; i++) {
		result += resultsOfThreads[i];
	}

	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	printf("Total execution time for current run: %f \n", std::chrono::duration <double, std::milli>(diff).count());

	result = (long double)((long double)(4 * 882) / (long double)result);
	return result;
}

void benchmark(int threads, int iterations, int _testCount) {

	threadsCount = threads;
	iterationsCount = iterations;
	testCount = _testCount;


	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < testCount; i++) {
		InitMap();
		double result = InitThreads();
	}

	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << "threads: " << threadsCount << "   iterations: " << iterationsCount << "   ";
	std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
}

void parseArgs(int argc, const char * argv[]) {
	CLParser cmd_line(argc, argv, true);

	iterationsCount = cmd_line.get_arg_int("-p");
	threadsCount = cmd_line.get_arg_int("-t");
	fileName = cmd_line.get_arg("-o");
	quiet = cmd_line.get_arg_int("-q");

	if (iterationsCount == -1) iterationsCount = 1;
	if (threadsCount == -1) threadsCount = 1;
}

void saveOutputInFile(long double num)
{
	if (fileName != "") {
		std::ofstream myfile;
		myfile.open(fileName);
		myfile << std::setprecision(100) << num << "\n";
		myfile.close();
	}
}

int main(int argc, const char * argv[]) {
	threadsCount = 2;
	iterationsCount = 2;
	parseArgs(argc, argv);

	long double result = InitThreads();
	saveOutputInFile(result);


	
	return 0;
}

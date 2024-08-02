#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

extern unsigned long long elapsed_ns;
extern struct timespec start, fini;

void interface(string variation, int value);

#endif // UTILS_H
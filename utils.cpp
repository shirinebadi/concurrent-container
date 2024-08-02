#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include "variation.h"
#include "utils.h"
#include <string.h>

using namespace std;

unsigned long long elapsed_ns;
struct timespec start, fini;

template <typename T>
void pop(T &x, int value) {
    #pragma omp parallel
{       
        int res;
        int counter = 0;
        while(true){
        while(!x.isEmpty()){
            x.pop(res);
            counter ++;
            }
        if(counter > value/2){
            cout <<"cancel" <<endl;
            break;
            }
        }
        #pragma omp cancel parallel
}
    return;
}

template <typename T>
void push(T &x, int value) {
    #pragma omp parallel for
    for (int i = 1; i <= value; ++i) {
        x.push(i);
    }
}

template <typename T>
void enqueue(T &x, int value) {
    #pragma omp parallel for
    for (int i = 1; i <= value; ++i) {
        x.enqueue(i);
    }
}

template <typename T>
void dequeue(T &x, int value) {
    #pragma omp parallel for
    for (int i = 1; i <= value; ++i) {
        int res;
        while (!x.dequeue(res)){}
    }
}

// int pop_util(){
//     int x;

// 	elapsed_ns = (fini.tv_sec-start.tv_sec)*1000000000 + (fini.tv_nsec-start.tv_nsec);
// 	printf("Push/Enqueue Time Elapsed (ns): %llu\n",elapsed_ns);
//     double elapsed_s = ((double)elapsed_ns)/1000000000.0;
// 	printf("Push/Enqueue Time Elapsed (s): %lf\n",elapsed_s);

//     cout << "Do you want to pop/dequeue? 1/0\n";
//     cin >> x;

//     return x;
// }

void interface(string variation, int value){
int x;

if (variation ==  "SGLStack") {
    SGLStack stack_;
    #pragma omp parallel
{
    #pragma omp master
    {
    #pragma omp task
    {
    push <SGLStack> (stack_, value);
    }
    #pragma omp task
    {
    pop <SGLStack> (stack_, value);
    }
    }
}
}

if (variation ==  "SGLQueue") {
    SGLQueue queue_;
    #pragma omp parallel
{
    #pragma omp master
    {
    #pragma omp task
    {
    enqueue <SGLQueue> (queue_, value);
    }
    #pragma omp task
    {
    dequeue <SGLQueue> (queue_, value);
    }
}
}
    return;
}

if (variation ==  "TStack") {
    TreiberStackWithElimination stack_;
    #pragma omp parallel
{
    #pragma omp master
    {
    #pragma omp task
    {
    push <TreiberStackWithElimination> (stack_, value);
    }
    #pragma omp task
    {
    pop <TreiberStackWithElimination> (stack_, value);
    }
    #pragma omp taskwait
    }
}
    return;
}

if (variation ==  "MSQ") {
    MichaelScottQueue queue;

    #pragma omp parallel
{
    #pragma omp master
    {
        #pragma omp task
        {
            enqueue <MichaelScottQueue> (queue, value);
        }
        #pragma omp task
        {
            dequeue <MichaelScottQueue> (queue, value);
        }
}
}
    return;
}

return;
}


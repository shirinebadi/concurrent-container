#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <string.h>
#include <omp.h>  
#include "utils.h"

using namespace std;

#define Description "This is an implementation of SGL lock Q and Stack, in addition to Treiber stack with elimination and Michael&Scott queue. Initially, You can only push or enqueue numbers from 1 to any value. Then you can decide if you want to pop/dequeue them.  \nUse '--name' to find out about the developer \n \n Input value (int) [-i] \n Variation [-v SGLStack/SGLQueue/MSQ/TStack] \n e.g. -i 1000 -v SGLStack\n"

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char * argv[]){

    const char *barrier = "";
    const char *lock = "";
    

    if(cmdOptionExists(argv, argv + argc, "--name")){
        cout << "Shirin Ebadi";
        return 0;
    }

    if(cmdOptionExists(argv, argv + argc, "-h")){
        cout << Description;
        return 0;
    }

    int number_threads = 4;

    int value = atoi(getCmdOption(argv, argv + argc, "-i"));
    if(cmdOptionExists(argv, argv + argc, "-t")){
       number_threads = atoi(getCmdOption(argv, argv + argc, "-t"));
    }

    char * variation = getCmdOption(argv, argv + argc , "-v");
    omp_set_dynamic(0); 
    omp_set_num_threads(number_threads); 

    clock_gettime(CLOCK_MONOTONIC,&start);
    interface(variation, value);
    clock_gettime(CLOCK_MONOTONIC,&fini);

    elapsed_ns = (fini.tv_sec-start.tv_sec)*1000000000 + (fini.tv_nsec-start.tv_nsec);
	printf("Pop/Dequeue Time Elapsed (ns): %llu\n",elapsed_ns);
    double elapsed_s = ((double)elapsed_ns)/1000000000.0;
	printf("Pop/Dequeue Time Elapsed (s): %lf\n",elapsed_s);
}


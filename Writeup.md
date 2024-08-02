# Experimental results

Following results are for 4 threads and 1-10,000,000 input.

| Primitive    | Runtime (s) |
|--------------|-------------|
| SGL Queue              |  2.79 | 
| SGL Stack W/ Eliminaition        | 5.48 | 
| Trieber Stack W/ Elimination          | 2.10 | 
| MSQ           | 1.87 | 

Following results are for 2 threads and 1-10,000,000 input.

| Primitive    | Runtime (s) |
|--------------|-------------|
| SGL Queue              |  2.65 | 
| SGL Stack W/ Eliminaition        | 4.24  | 
| Treiber Stack W/ Elimination          | 2.026  | 
| MSQ           | 1.96 | 

Following results are for 6 threads and 1-10,000,000 input.

| Primitive    | Runtime (s) |
|--------------|-------------|
| SGL Queue              |  2.69 | 
| SGL Stack W/ Eliminaition        | 4.383 | 
| Treiber Stack W/ Elimination          | 2.04  | 
| MSQ           | 1.91 | 

# Analysis of results

Treiber stach and M&S queue have the best results among others. It is predicitable, though, since Treiber Stack and MSQ are designed to be lock-free and have better concurrent handling so they outperform SGL Queue and SGL Stack in scenarios with multiple threads. It seems like there is no significant difference between 2-6 threads.

Following is our perf cache-miss rate obeservation for 4 threads:

| Primitive    | Cache Miss Rate |
|--------------|-------------|
| SGL Queue              |  0.905  | 
| SGL Stack W/ Eliminaition        | 5.469 % | 
| Trieber Stack W/ Elimination          |  3.435 %  | 
| MSQ           | 1.658 % | 

for 6 threads:

| Primitive    | Cache Miss Rate |
|--------------|-------------|
| SGL Queue              |  0.780 % | 
| SGL Stack W/ Eliminaition        | 5.353 %  | 
| Trieber Stack W/ Elimination          |  3.388 %  | 
| MSQ           |  1.268 %  | 

Surprisingly, adding elimination to stacks increased the cache miss rate. My assumption is that although elimination is useful, but its implementation can has a significant effect. Therefore, I guess my implementation is not very efficient. However, both Treiber stack and MS queue have good performance which shows that concurrent data structers despite their high cache miss rate execute faster. 


# A description of your code organization

My code organization is based on "kind". I see each Qeue and Stack as a componenet (Class). I also have some functions as utils and an entry point in main.

# A description of every file submitted

I have 4 main files:
    1. variation.h: Containes classes of different implementations.
    2. utils.cpp: Containes utils that provide essential interface for calling these classes.
    3. utils.h: Header file on utils.cpp which provides a function for main.
    4. main.cpp: Containes the entry point of the program.

# Compilation instructions

After cloning the project, go to the project direct and execute 'make' in terminal.

# Execution instructions

After compiling the code, you can run the program using following command:

```
./main -i [input value] -v [variation TStack/MSQ/SGLStack/SGLQueue] -t [number of threads]
e.g: ./main -i 10000 -v TStack -t 3

```

# Any extant bugs



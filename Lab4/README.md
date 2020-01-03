# Goal
The goal of this lab is to put together a more complex scenario using threads, mutual exclusion, and producere-consumer synchronization.

The programs to be written will demonstrate:

the usage of threads to divide the work;
the usage of mutexes to protect the invariants on the shared data
the usage of condition variables to signal, from the producer side, of the availability of data for the consumer side.
#Requirement
Parallelize the multiplication of 3 matrices. Use a configurable number of threads to do one matrix multiplication. Then, use another set of threads to do the second multiplication. The threads in the second set should start as soon as they start having data from the first multiplication result.
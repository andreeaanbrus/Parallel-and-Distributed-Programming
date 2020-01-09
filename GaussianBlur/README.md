# Gaussian Blur

### Run the program
Parallel: compile + run   
Distributed: ``` cd cmake-build-debug/ && time mpiexec -n 8 ./GaussianBlur distributed ```
### Input conditions:
Kenrel: 5
No of threads: 8 

### Results   
| Image Size  | Linear | Threads | MPI | 
| ------------- | ------------- | ------------- | ------------- |
| Content Cell  | Content Cell  |
|  800 x 650 | 3.4s | 2.4s | 1.9s|  
| 1800 x 1200 | 17.32s | 19.27s | 10.92s
| 4000 x 4000  | 126.42s  | 128.29s | 184.94s

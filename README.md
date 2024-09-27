# :milky_way: StarGazer - MPI

StarGazer-MPI is a parallel application designed to count stars in astronomical images using distributed processing with MPI (Message Passing Interface). The tool splits the image into blocks and distributes the workload across multiple processes, speeding up the analysis of large datasets. 

```shell
#Compile the mpi program
make

#Execute the program
make run p=<number of process> img=<path/to/image.pgm>
```

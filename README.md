# :milky_way: StarGazer - MPI

**StarGazer-MPI** is a parallel application designed to count stars in astronomical images using distributed processing with **MPI** (Message Passing Interface). This tool splits images into blocks and distributes the workload across multiple processes, significantly speeding up the analysis of large datasets.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Usage](#usage)
- [Generating Histogram](#generating-histogram)
- [Help](#help)

## Prerequisites

Before running the project, ensure you have the following installed:

- [MPI (Message Passing Interface)](https://www.open-mpi.org/software/ompi/v5.0/)
- [Python 3](https://www.python.org/downloads/source/)
- [pip](https://pypi.org/project/pip/)
- [GCC](https://gcc.gnu.org/install/)
- [GNU Make](https://www.gnu.org/software/make/)

## Usage

This project can be easily built using a makefile. Follow the commands below to compile and run the application:

> :warning: Commands must be executed in the root directory.

```shell
# Compile the MPI program
make

# Execute the program
make run p=<number_of_processes> img=<path/to/image.pgm> th=<threshold>

# Clean up the build
make clean
```

## Generating Histogram

To help you choose the appropriate threshold value, you can generate a histogram plot (requires Python). Use the following commands:

```shell
# Build the Python virtual environment and install dependencies
make build

# Execute the histogram generation
make hist img=<path/to/image.pgm>

# Clean up the environment
make purge

```

<img src="https://github.com/otaviofbrito/StarGazer-MPI/blob/misc/img/plot_hist.png" alt="Histogram"/>

## Help

For additional help and available commands, run:

```shell
make help
```

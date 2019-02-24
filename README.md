# Parallel Quicksort

## Compiling

Tested on Apple LLVM Clang version 10, GCC versions 5, 6, 7, 8. Requires C++11. Portable to unix systems (tested macOS, Ubuntu). Dockerfile used for testing is included.

```
$ clang++ -std=c++11 -O -o main *.cpp -pthread
# alternatively
$ g++ -std=c++11 -O -o main *.cpp -pthread

$ ./main input.csv output.csv 1,2 16
```

## Structure

I used a fork of a popular single-header ThreadPool library, available [here](https://github.com/log4cplus/ThreadPool). License is included the git repository. It is included as "ThreadPool.h"

I was not able to find a small/flexible enough CSV library, so I ran with a short implementation of my own. Files "CSVReader", "CSVWriter", "CSVLine", "CSVValue" (.h,cpp).

I made a wrapper class around the primitive/unsafe utilities for creating temporary files, "TempFile.h,cpp".

The implementation of Quicksort uses templates and as such is a single header file "Quicksort.h".

The main sorting logic (breaking input into chunks, merging them back together) is in "CSVSorter.h,cpp".

## Performance

This implementation performs worse than the standard `sort` utility available on most unix machines. This is pretty expected, as `sort` is very well optimized. Still, I consider this performance satisfactory, and it does improve when allowed more threads (up to a certain point).

For the sample data fetched from Mars DB
```
$ wc -l input.csv
  360000 input.csv

# unix sort - baseline
$ time sort -t\, -k 2,2n -k 3,3n -k 1,1d input.csv > output_sort.csv
  3.02s user 0.12s system 99% cpu 3.145 total

# quicksort - 16 threads
$ time ./main input.csv output_isg.csv 1,2,0 16
  4.54s user 2.41s system 119% cpu 5.838 total

# quicksort - 1 thread
$ time ./main input.csv output_isg.csv 1,2,0 1
  4.58s user 2.15s system 98% cpu 6.804 total
```

For an expanded data set (Mars DB concatted 5x)
```
$ wc -l input_large.csv
  1800000 input_large.csv

# unix sort - baseline
$ time sort -t\, -k 2,2n -k 3,3n -k 1,1d input_large.csv > output_sort_large.csv
  26.41s user 0.60s system 99% cpu 27.131 total

# quicksort - 16 threads
$ time ./main input_large.csv output_isg_large.csv 1,2,0 16
  25.99s user 11.25s system 128% cpu 29.077 total

# quicksort - 1 thread
$ time ./main input_large.csv output_isg_large.csv 1,2,0 1
  24.58s user 11.41s system 98% cpu 36.361 total
```

## Tunability

There are a few parameters in this sort which could be tuned to a dataset/machine to increase performance.

- Chunk size, default 100,000, is the number of lines read at a time before splitting into temporary files.
    This is almost 100% dependent on amount of ram a machine has. The bigger the chunk size, the more
    time is spent in the parallelized quicksort as opposed to the io-heavy merge step.
- Thread count, parameter, is the number of threads used in the quicksort thread pool.
    This should just be the number of cpu cores your machine has (or double if hyperthreading), any more
    and there are no gains from threadpooling.
- Sequential quicksort threshold, default 100, is the size of the subarray after which quicksort switches from parallel to sequential.
    The optimal number here can be experimentally determined but a default of 100 seems to work just fine.

## Correctness

I compared results from my parallel quicksort implementation to the built-in `sort` via the built-in `diff`. That is,

```
$ diff output_isg.csv output_sort.csv
$ diff output_isg_large.csv output_sort_large.csv
```

In all tested cases the results were identical (so the sort seems to be correct).

## Profiling Notes

In attempt to improve runtime, I used a sampling profiler to analyze performance bottlenecks. My findings were as follows:

- Only ~30% of runtime is spent actually sorting the data.
    Sorting work is evenly distributed amongst cores (threads).
    This sorting is relatively efficient, with most of the cpu cycles going towards the partition method (not allocations, etc).
    I did my best to optimize hot spots (specifically, the comparator).
- Much of the time (~35%) goes to file system operations.
    That is, reading/writing the input, output, and temporary files.
    Unfortunately this is not particularly trivial to optimize, and seems out of the scope for this assignment.
- Most of the rest goes to various vector allocations and string operations in CSVReader and CSVWriter.
    Although this is potentially trivial to optimize, I still considered it out of the scope for this assignment,
    as dropping in a robust CSV library would take care of this issue (I was unable to find a decent single-header one).

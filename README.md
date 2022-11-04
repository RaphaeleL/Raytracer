# Raytracer

## Run 

This Code should run on every Architecture. The Run Commands down below are for Intel Linux Systems.

```bash
g++ -Wall -pedantic -march=native -mfpmath=sse -mavx -O3 -fprofile-arcs -ftest-coverage -pg raytracer.cc statistics.cc
```

To run the Profiler use following Command (also for Intel Linux Systems):

```bash
gcov raytracer.cc 
gprof > tayler.gprof
```

now you can open the Generated Profiling Files in an Editor of your Choice

# Raytracer

## Run

This Code should run on every Architecture. The Run Commands down below are for Intel Linux Systems.

```bash
g++ -Wall -pedantic -march=native -mfpmath=sse -mavx -O3 -fprofile-arcs -ftest-coverage -pg raytracer.cc statistics.cc
```

for teaching purposes, an optimized and a non-optimized variant are implemented in some places. to understand the differences, the profile can be executed as follows. to compile the optimized version append `-d OPTIMIZED_INTERSECTS` to your `g++`-Command

```bash
gcov raytracer.cc
gprof > tayler.gprof
```

now you can open the Generated Profiling Files in an Editor of your Choice

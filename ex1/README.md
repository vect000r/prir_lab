# PRiR ex1

Starter implementation for the first parallel and distributed programming
exercise.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

The executable is created at `build/ex1`.

## Run

```bash
./build/ex1
```

The starter currently contains the sequential implementation. The MPI
implementation can be added as another `DataProcessor` implementation without
changing the shared interfaces.

# Third Party Tools & Libraries

## Table of contents

- [Third Party Tools & Libraries](#third-party-tools--libraries)
  - [Table of contents](#table-of-contents)
  - [Tools](#tools)
    - [CMake](#cmake)
    - [CPPCheck](#cppcheck)
  - [Libraries](#libraries)
    - [Benchmark](#benchmark)

## Tools

### CMake

<https://github.com/Kitware/CMake>

#### Build & Install CMake

```bash
git clone --depth=1 --branch v3.30.5 --recurse-submodules -j$(nproc) git@github.com:Kitware/CMake.git cmake-3.30.5
cd cmake-3.30.5 && mkdir build && cd build
../bootstrap
make -j$(nproc)
sudo make install
cmake --version
```

### CPPCheck

<https://github.com/danmar/cppcheck>

#### Build & Install CPPCheck

```sh
git clone --depth=1 --branch 2.16.0 --recurse-submodules -j$(nproc) https://github.com/danmar/cppcheck.git cppcheck-2.16.0
cd cppcheck-2.16.0 && mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
sudo make install
```

## Libraries

### Benchmark

<https://github.com/google/benchmark>

#### Build & Install Benchmark

```bash
git clone --depth=1 --branch v1.9.0 --recurse-submodules -j$(nproc) git@github.com:google/benchmark.git bench-1.9.0
cd bench-1.9.0 && mkdir build && cd build
make -j$(nproc)
sudo make install
cmake --version
```

# Third Party Tools & Libraries

## Table of contents

- [Third Party Tools & Libraries](#third-party-tools--libraries)
  - [Table of contents](#table-of-contents)
  - [Tools](#tools)
    - [CMake](#cmake)
    - [CCache](#ccache)
    - [CPPCheck](#cppcheck)
  - [Libraries](#libraries)
    - [Benchmark](#benchmark)
    - [Google Test](#google-test)

## Tools

### CMake

<https://github.com/Kitware/CMake>

#### Build & Install CMake

```bash
git clone --depth=1 --branch v4.1.0 --recurse-submodules -j$(nproc) git@github.com:Kitware/CMake.git cmake-4.1.0
cd cmake-4.1.0 && mkdir build && cd build
../bootstrap
cmake --build . --parallel
sudo cmake --install .
cmake --version
```

If you have upgraded to the latest copiler you may need to do the following:

```sh
sudo LD_LIBRARY_PATH=/usr/local/gcc-14.1.0/lib64:$LD_LIBRARY_PATH make install
```

### CCache

<https://github.com/ccache/ccache>
```bash
git clone --depth 1 --branch v4.12.1 --recurse-sudmobuldes -j$(nproc) git@github.com:ccache/ccache.git ccache-4.12.1
cd ccache-4.12.1 && mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
sudo cmake --install .
ccache --version
```

### CPPCheck

<https://github.com/danmar/cppcheck>

#### Build & Install CPPCheck

```sh
git clone --depth=1 --branch 2.18.3 --recurse-submodules -j$(nproc) https://github.com/danmar/cppcheck.git cppcheck-2.18.3
cd cppcheck-2.18.3 && mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
sudo cmake --install .
```

## Libraries

### Benchmark

<https://github.com/google/benchmark>

#### Build & Install Benchmark

```bash
git clone --depth=1 --branch v1.9.0 --recurse-submodules -j$(nproc) git@github.com:google/benchmark.git bench-1.9.0
cd bench-1.9.0 && mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
sudo cmake --install .
```

#### Google Test

<https://github.com/google/googletest>

```bash
git clone --depth 1 --branch v1.17.0 --recurse-submodule git@github.com:google/googletest.git google-test-1.17.0
cd google-test-1.17.0 && mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
sudo cmake --install .
```

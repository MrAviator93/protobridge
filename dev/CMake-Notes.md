# CMake

Quick cmake build from source notes.

## Build & Install CMake 3.30.5

<https://github.com/Kitware/CMake>

```bash
git clone --depth=1 --branch v3.30.5 --recurse-submodules -j$(nproc) git@github.com:Kitware/CMake.git cmake-3.30.5
cd cmake-3.30.5 && mkdir build && cd build
../bootstrap
make -j$(nproc)
sudo make install
cmake --version
```

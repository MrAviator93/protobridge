# CMake

Quick cmake build from source notes.

## CMake 3.28

<https://github.com/Kitware/CMake>

```bash
git clone --depth=1 --branch v3.29.3 --recurse-submodules -j$(nproc) git@github.com:Kitware/CMake.git cmake-3.29.3
cd cmake-3.29.3 && mkdir build && cd build
../bootstrap
make -j$(nproc)
sudo make install
cmake --version
```

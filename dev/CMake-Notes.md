# CMake

Quick cmake build from source notes.

## CMake 3.28

<https://github.com/Kitware/CMake>

```bash
git clone --depth=1 --branch v3.28.0 --recurse-submodules -j20 git@github.com:Kitware/CMake.git cmake-3.28.0
cd cmake-3.28.0 && mkdir build && cd build
../bootstrap
make -j$(nproc)
sudo make install
cmake --version
```

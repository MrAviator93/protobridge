# Raspberry Pi Related Notes

## Building & Installing GCC-13.2 Tool-set

This section provides a detailed walkthrough for installing the GCC-13.2 compiler on a Raspberry Pi. This process is essential for developers looking to utilize the latest features of GCC on Raspberry Pi hardware.

### Initial Setup

Start by updating your system and installing the necessary building tools and libraries

```sh
sudo apt update
sudo apt install build-essential
sudo apt-get install libgmp3-dev libmpfr-dev libmpc-dev
```

### Downloading GCC-13.2

Fetch the GCC-13.2 source code into a specific directory (not that this directory must be writable by your user, it doesn't require sudo permissions) and extract it.

```sh
mkdir -p ~/gcc-builds && cd ~/gcc-builds
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xvf gcc-13.2.0.tar.gz
```

## Preparing for Compilation

Navigate to the GCC directory and prepare the build environment

```sh
cd gcc-13.2.0 && mkdir build && cd build
```

### Configuring the Build

Configure the GCC build with specific parameters for the Raspberry Pi architecture:

```sh
../configure -v --build=aarch64-linux-gnu \
				--host=aarch64-linux-gnu \
				--target=aarch64-linux-gnu \
				--disable-multilib \
				--prefix=/usr/local/gcc-13.2.0 \
				--enable-checking=release \
				--enable-default-ssp  \
				--disable-fixincludes \
				--enable-languages=c,c++,fortran,objc,obj-c++
```

### Compiling GCC

Compile the source code using all available cores for efficiency

```sh
make -j$(nproc)
```

### Installation

Install the newly built GCC (will install into /usr/local/gcc-13.2.0)

```sh
sudo make install 
```

### Configuring the Environment

Update the LD_LIBRARY_PATH environment variable to include the path to the newer libstdc++ and also update PATH to include the new compiler.

```sh
export LD_LIBRARY_PATH=/usr/local/gcc-13.2.0/lib64:$LD_LIBRARY_PATH
export PATH=/usr/local/gcc-13.2.0/bin:$PATH
```

Update the dynamic linker run-time bindings

```sh
sudo ldconfig
```

Set the new GCC version as the default compiler

```sh
sudo update-alternatives --install /usr/bin/gcc gcc /usr/local/gcc-13.2.0/bin/gcc 60 --slave /usr/bin/g++ g++ /usr/local/gcc-13.2.0/bin/g++
```

### Verifying the Installation

Confirm the installation by checking the version of GCC and G++

```sh
gcc --version
g++ --version
```

### Notes

- Building GCC-13.2 on a Raspberry Pi 5 typically takes just over 2 hours. Expect longer build times on a Raspberry Pi 4.
- It is recommended to monitor the temperature and performance of your Raspberry Pi during the build process to prevent overheating.

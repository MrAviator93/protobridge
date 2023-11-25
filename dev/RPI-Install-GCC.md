
# Raspberry Pi Related Notes

## Building & Installing GCC-13.2 Toolset

```sh
sudo apt update
sudo apt install build-essential
sudo apt-get install libgmp3-dev libmpfr-dev libmpc-dev
```

```sh
wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz
tar -xvf gcc-13.2.0.tar.gz
```

```sh
cd gcc-13.2.0 && mkdir build && cd build
```

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

```sh
make -j$(nproc)
```

Install the build (will install into /usr/local/gcc-13.2.0)

```sh
sudo make install 
```

You also need to do the following operation:

```sh
export LD_LIBRARY_PATH=/usr/local/gcc-13.1.0/lib64:$LD_LIBRARY_PATH
```

As the new dynamic libraries are located in different location, the path needs to be provided for dynamic linker where to look for them.

### Useful Resources

https://iq.opengenus.org/build-gcc-from-source/
https://www.linuxfromscratch.org/blfs/view/svn/general/gcc.html

### Notes

Note that building GCC-13.2 for Raspberry Pi 5 takes a bit more than 2 hours, for Raspberry Pi 4 it will take even more.

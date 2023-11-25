ARG PBL_MAJOR_VERSION = 1
ARG PBL_MINOR_VERSION = 0
ARG PBL_PATCH_VERSION = 0

FROM debian:12 AS pbl-base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt install -y build-essential && apt-get clean

FROM pbl-base AS pbl-dev-base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y libgmp-dev libmpfr-dev libmpc-dev wget

# Download and extract GCC
RUN wget https://ftp.gnu.org/gnu/gcc/gcc-13.2.0/gcc-13.2.0.tar.gz && \
    tar -xvf gcc-13.2.0.tar.gz

# Configure and start the build process
RUN cd gcc-13.2.0 && mkdir build && cd build && \
    ../configure -v \
    --build=x86_64-linux-gnu \
    --host=x86_64-linux-gnu \
    --target=aarch64-linux-gnu \
    --disable-multilib \
    --prefix=/usr/local/gcc-13.2.0 \
    --enable-checking=release \
    --enable-default-ssp \
    --disable-fixincludes \
    --enable-languages=c,c++



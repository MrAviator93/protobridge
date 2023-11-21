ARG PBL_MAJOR_VERSION = 1
ARG PBL_MINOR_VERSION = 0
ARG PBL_PATCH_VERSION = 0

FROM debian:12 AS pbl-cpp-base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt install -y build-essential && apt-get clean
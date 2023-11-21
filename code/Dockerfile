ARG BBX_MAJOR_VERSION = 1
ARG BBX_MINOR_VERSION = 0
ARG BBX_PATCH_VERSION = 0

FROM debian:12 AS cpp-base
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt install -y build-essential && apt-get clean
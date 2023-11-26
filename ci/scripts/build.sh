#!/bin/bash

# Stop on the first sign of trouble
set -e

# Echo each command
set -x

# Build for each target platform
docker build -f ci/docker/Dockerfile.beaglebone -t pbl/beaglebone .
docker build -f ci/docker/Dockerfile.orangepi -t pbl/orangepi .
docker build -f ci/docker/Dockerfile.x86_64 -t pbl/x86_64 .
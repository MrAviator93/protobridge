#!/bin/bash

# Stop on the first sign of trouble
set -e

# Echo each command
set -x

# Build for each target platform
docker build -f ci/docker/Dockerfile.beaglebone -t yourproject/beaglebone .
docker build -f ci/docker/Dockerfile.orangepi -t yourproject/orangepi .
docker build -f ci/docker/Dockerfile.x86_64 -t yourproject/x86_64 .
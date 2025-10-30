# Docker

This document describes how to set up and use Docker for building and testing ProtoBridge components in isolated and reproducible environments.

## What Is Docker?

Docker is a containerization platform that allows you to package software with all its dependencies (libraries, tools, configuration) into lightweight, portable containers. Each container runs in isolation but can communicate with others via well-defined interfaces - ideal for CI pipelines and cross-compilation setups.

For an introduction and installation instructions, follow the official guide: <https://docs.docker.com/engine/install/ubuntu/>

## Initial Set-Up

After installing Docker Engine, install qemu-user-static - this enables running containers for non-native architectures (for example, ARM containers on x86 or x64 hosts).

```sh
sudo apt-get update -y
sudo apt-get install -y qemu-user-static
```

## User Groups

To run Docker without sudo, your user must belong to the docker group. You may also want to create and use an additional group (e.g., pbl) for internal project permissions.

### Add Groups and Assign Users

```sh
sudo groupadd docker   # if not already present
sudo groupadd pbl      # optional group for ProtoBridge builds
sudo usermod -aG docker $USER
```

**Note: Replace $USER with your username if not running interactively.**

- Apply group changes without reboot: `newgrp docker`.
- To list all users use simply: `groups`.

## Common Docker Commands

Below are frequently used commands during development and CI.

| Command | Description |
| --- | --- |
| `docker images` | List all available local images |
| `docker ps -a` | List all containers (including stopped ones) |
| `docker system prune` | Clean up unused images, containers, and networks |
| `docker build -t <image-name> .` | Build an image from the current directory’s Dockerfile|
| `docker run -it <image-name> /bin/bash` | Start a container interactively with a Bash shell |

Example:

```sh
docker build -t pbl-cross-bbb:trixie .
docker run -it pbl-cross-bbb:trixie /bin/bash
```

## Backing Up Docker Images

Create a backup that can then be used with docker load docker <https://docs.docker.com/engine/reference/commandline/save/>

### Save Images

To back up or transfer your ProtoBridge cross-compile images, use docker save.
This exports the image into a .tar file that can later be re-loaded on another machine.

```sh
# Save the image as a tarball
docker save pbl-cross-bbb:trixie > pbl-cross-bbb_trixie.tar

# Verify the file size
ls -sh pbl-cross-bbb_trixie.tar
```

Alternate syntax:

```sh
docker save --output pbl-cross-bbb_trixie.tar pbl-cross-bbb:trixie
```

Save multiple tags or images

```sh
docker save -o pbl-cross-all.tar pbl-cross-bbb:trixie pbl-cross-rpi2w:trixie
```

### Save & Compress Images (Recommended)

You can use gzip to save the image file and make the backup smaller.

```sh
docker save myimage:latest | gzip > myimage_latest.tar.gz
docker save pbl-cross-bbb:trixie | gzip > pbl-cross-bbb_trixie.tar.gz
docker save pbl-cross-rpi2w:trixie | gzip > pbl-cross-rpi2w_trixie.tar.gz
```

### Restoring Docker Images

To load the image refer to the following document docker load for details. <https://docs.docker.com/engine/reference/commandline/load/>

```sh
# List current images
docker image ls

# Load from a compressed archive
gunzip -c pbl-cross-bbb_trixie.tar.gz | docker load

# or directly from an uncompressed file
docker load < pbl-cross-bbb_trixie.tar

# Verify the restored image
docker images | grep pbl-cross-bbb

```

Restore multiple images:

```sh
docker load < pbl-cross-all.tar
```

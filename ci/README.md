# CI

## Docker

Docker is a set of platform as a service products that use OS-level virtualization to deliver software in packages called containers. Containers are isolated from one another and bundle their own software, libraries and configuration files; they can communicate with each other through well-defined channels.

For initial Docker set-up follow the following guide: <https://docs.docker.com/engine/install/ubuntu/>

### Set-Up

After that you will need to install the following components:

Install qemu-user-static:

```sh
sudo apt-get update -y
sudo apt-get install -y qemu-user-static
```

User Groups

You also need to add a docker to groups as well as users which is created inside the container, for our case the user is vad .

To add new users to group:

```sh
sudo groupadd docker
sudo groupadd pbl
sudo usermod -aG docker $USER
```

To list all users use simply:

```sh
groups
```

### Useful Commands

```bash
docker images
```

```bash
docker ps -a
```

```bash
docker system prune
```

```bash
docker build -t <docker-image-name> .
```

```bash
docker run -it <docker-image-name> /bin/bash
```

Backup

Create a backup that can then be used with docker load docker <https://docs.docker.com/engine/reference/commandline/save/>

```bash
 docker save busybox > busybox.tar
 ls -sh busybox.tar
 docker save --output busybox.tar busybox
 ls -sh busybox.tar
 docker save -o fedora-all.tar fedora
 docker save -o fedora-latest.tar fedora:latest
```

Or another way of doing the above would be to use gzip. You can use gzip to save the image file and make the backup smaller.

```bash
docker save myimage:latest | gzip > myimage_latest.tar.gz
```

Load

To load the image refer to the following documentdocker load for details. <https://docs.docker.com/engine/reference/commandline/load/>

```bash
docker image ls
docker load < busybox.tar.gz
```

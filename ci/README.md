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
docker build -t <docker-image-name> .
```

```bash
docker run -it <docker-image-name> /bin/bash
```
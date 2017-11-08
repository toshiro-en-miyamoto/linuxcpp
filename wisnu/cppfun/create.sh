#!/bin/sh
echo 'Removing the container gcc7'
docker rm -f gcc7
echo 'Creating the container gcc7'
docker create \
  --name=gcc7 \
  --user=`id -u`:`id -g` \
  --volume=`pwd`:/usr/src/myapp \
  --workdir=/usr/src/myapp \
  -it \
  gcc:7.2 /bin/bash
echo 'Starting the container gcc7'
docker start \
  gcc7

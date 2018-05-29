
## IMPORTANT: For create the docker image, launch "docker build -t tag ." tag is the name of the docker image
## IMPORTANT: Replace "docker_gaw" with the name of your docker image name.

xhost local:root
xhost +local:docker

docker run -i -t --name test_1 --privileged=true \
-v /etc/machine-id:/etc/machine-id:ro -v /var/run/dbus:/var/run/dbus \
-v /tmp/.X11-unix:/tmp/.X11-unix:rw -v /dev/shm:/dev/shm \
-p x.x.x.x:8555:8555/udp \
-p x.x.x.x:8553:8553 \
-p x.x.x.x:8554:8554 \
-p x.x.x.x:6970:6970/udp \
-p x.x.x.x:6971:6971/udp \
-p x.x.x.x:8080:8080 \
-p x.x.x.x:8888:8888 \
--ipc=host \
--device /dev/dri/card0 -e DISPLAY=unix$DISPLAY docker_gaw /bin/bash

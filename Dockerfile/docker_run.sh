
## IMPORTANT: For create the docker image, launch "docker build -t tag ." tag is the name of the docker image
## IMPORTANT: Replace "docker_gaw" with the name of your docker image name.

xhost local:root
xhost +local:docker

docker run -i -t --name ffmpeg-control-api-1 --privileged=true \
-v /etc/machine-id:/etc/machine-id:ro -v /var/run/dbus:/var/run/dbus \
-v /tmp/.X11-unix:/tmp/.X11-unix:rw -v /dev/shm:/dev/shm \
-p 127.0.0.1:8555:8555/udp \
-p 127.0.0.1:8553:8553 \
-p 127.0.0.1:8554:8554 \
-p 127.0.0.1:6970:6970/udp \
-p 127.0.0.1:6971:6971/udp \
-p 127.0.0.1:8080:8080 \
-p 127.0.0.1:8888:8888 \
--ipc=host \
--device /dev/dri/card0 -e DISPLAY=unix$DISPLAY ffmpeg-control-api /bin/bash

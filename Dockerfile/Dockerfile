
## IMPORTANT: For create the docker image, launch "docker build -t tag ." tag is the name of the docker image

FROM ubuntu:16.04

WORKDIR /home/admin

ENV LD_LIBRARY_PATH=/home/admin/FFMPEG-Control-API-Arqueopterix/_install_dir_x86/lib

# Expose necessary ports
EXPOSE 8553/TCP
EXPOSE 8554/TCP
EXPOSE 8555/TCP
EXPOSE 6970/UDP
EXPOSE 6971/UDP
EXPOSE 8080/TCP
EXPOSE 8888/UDP

RUN apt-get update && \
	apt-get install -y git \
	nano \
	curl \ 
	apt-utils \
	libc6-dbg \
	wget \
	build-essential \
	automake \
	libtool \
	vim \
	libx11-dev \
	libxext-dev \
	gcc \
	wmctrl \
	patch \
	make \
	cmake \
	g++ \
	pkg-config \
	libxtst-dev \
	libfreetype6-dev \
	libgl1-mesa-dev \
	libglu1-mesa-dev \
	libpulse-dev \
	libasound2-dev \
	lib32z1 \
	xvfb

# Download the code source from GitHub (FFMPEG-Control-API and 3rdplibs)
RUN	git clone --recursive https://github.com/MarioSanzRodrigo/FFMPEG-Control-API-Arqueopterix.git && \
	cd FFMPEG-Control-API-Arqueopterix && make 

# Scripts for execute server and client
ADD server.sh /home/admin/server.sh
ADD client.sh /home/admin/client.sh

# Env variables for GamingAnyWhere
ENV MEDIAPROCSDIR=/home/admin/FFMPEG-Control-API-Arqueopterix/_install_dir_x86
ENV GADEPS=$MEDIAPROCSDIR
ENV PKG_CONFIG_PATH=$GADEPS/lib/pkgconfig:/opt/local/lib/pkgconfig:/usr/lib/i386-linux-gnu/pkgconfig/:/usr/lib/pkgconfig
ENV PATH=$GADEPS/bin:$PATH

# Download the code source from GitHub (GamingAnyWhere)
RUN	cd /home/admin/FFMPEG-Control-API-Arqueopterix/gaminganywhere && cd ga && make all && make install

# Config file for GAW-MAME
ADD server.mlhe.mame.conf /home/admin/FFMPEG-Control-API-Arqueopterix/gaminganywhere/bin/config/server.mlhe.mame.conf

# Install MAME 
RUN apt-get install -y mame mame-extra mame-tools && \
	mkdir /home/admin/.mame && \
	mkdir /home/admin/roms

# NOTA el directorio del mame.ini es ~/.mame
# lanzar el comando /usr/games/mame -cc
# editar el mame.ini con la rompath /home/admin/roms/roms

# Download MAME ROMS
RUN wget https://www.dropbox.com/s/kg1rbqcn2a4x410/roms.tar.gz && \
	mv roms.tar.gz /home/admin/roms && \
	tar xzvf /home/admin/roms/roms.tar.gz

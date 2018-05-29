SRCPATH=../../../3rdplibs/x264
prefix=/home/mario/Documentos/Github_Test/1.ARQUEOPTERIX/FFMPEG-Control-API-Arqueopterix/_install_dir_x86
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
libdir=${exec_prefix}/lib
includedir=${prefix}/include
SYS_ARCH=X86_64
SYS=LINUX
CC=gcc
CFLAGS=-Wno-maybe-uninitialized -Wshadow -O3 -ffast-math -m64 -Wall -O3 -fPIC -D_LARGE_FILE_API -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Wall -O3 -fPIC -D_LARGE_FILE_API -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Wall -O3 -fPIC -D_LARGE_FILE_API -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -Wall -I. -I$(SRCPATH) -std=gnu99 -D_GNU_SOURCE -mpreferred-stack-boundary=6 -fPIC -fomit-frame-pointer -fno-tree-vectorize
COMPILER=GNU
COMPILER_STYLE=GNU
DEPMM=-MM -g0
DEPMT=-MT
LD=gcc -o 
LDFLAGS=-m64  -lm -lpthread -ldl
LIBX264=libx264.a
AR=gcc-ar rc 
RANLIB=gcc-ranlib
STRIP=strip
INSTALL=install
AS=nasm
ASFLAGS= -I. -I$(SRCPATH) -DARCH_X86_64=1 -I$(SRCPATH)/common/x86/ -f elf64 -DSTACK_ALIGNMENT=64 -DPIC -DHIGH_BIT_DEPTH=0 -DBIT_DEPTH=8
RC=
RCFLAGS=
EXE=
HAVE_GETOPT_LONG=1
DEVNULL=/dev/null
PROF_GEN_CC=-fprofile-generate
PROF_GEN_LD=-fprofile-generate
PROF_USE_CC=-fprofile-use
PROF_USE_LD=-fprofile-use
HAVE_OPENCL=yes
default: cli
install: install-cli
SOSUFFIX=so
SONAME=libx264.so.152
SOFLAGS=-shared -Wl,-soname,$(SONAME)  -Wl,-Bsymbolic
default: lib-shared
install: install-lib-shared
LDFLAGSCLI = 
CLI_LIBX264 = $(LIBX264)

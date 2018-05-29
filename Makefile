
#### GLOBAL DEFINITIONS #####
# Get **THIS** Makefile path:
# As make reads various makefiles, including any obtained from the MAKEFILES 
# variable, the command line, the default files, or from include directives, 
# their names will be automatically appended to the MAKEFILE_LIST variable. 
# They are added right before make begins to parse them. This means that if 
# the first thing a makefile does is examine the last word in this variable, 
# it will be the name of the current makefile. Once the current makefile has 
# used include, however, the last word will be the just-included makefile.
CONFIG_LINUX_MK_PATH_= $(shell pwd)/$(dir $(lastword $(MAKEFILE_LIST)))
GETABSPATH_SH= $(CONFIG_LINUX_MK_PATH_)/scripts/getabspath.sh
CONFIG_LINUX_MK_PATH= $(shell $(GETABSPATH_SH) $(CONFIG_LINUX_MK_PATH_))
INSTALL_DIR= $(CONFIG_LINUX_MK_PATH)/_install_dir_
3RDPLIBS= 3rdplibs

##### Define scripting #####
SHELL:=/bin/bash

##### Get platform host #####
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	ARCHS=x86 #arm mips
	SO_NAME=linux
endif
#ifeq ($(UNAME_S),Darwin) # e.g. use to add Darwing support
#	ARCHS= ... //TODO
#	SO_NAME=darwin
#endif
export SO_NAME

##### BASIC COMMON CFLAGS #####
CFLAGS+=-Wall -O3
#CFLAGS+=-Wall -g -O0
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
   # 64 bit stuff here
   CFLAGS+=-fPIC -D_LARGE_FILE_API -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
else
   # 32 bit stuff here
endif
export CFLAGS

.PHONY : $(3RDPLIBS)

#### Rules ####

# Make every target of the form '_<ARCHS array entry>'
# (Note: Substitution function '$(ARCHS:x=y)' substitutes very 'x' 
# pattern by replacement 'y')
all: $(ARCHS:=_)

# The MAKE and CLEAN macros:
# 1) $* will be substituted by each entry of the array ARCHS (e.g x86, ...)
# 2) _libname is a tag to be substituted by each library name 
# (e.g. ffmpeg, lame, ...) in the target
MAKE_MACRO__= $(MAKE) -C arch_$*_$(SO_NAME)/libname__ all CROSS_ARCH=$* \
	INSTALL_DIR=$(INSTALL_DIR)
MAKE_MACRO= $(MAKE_MACRO__:libname__=$(patsubst $*_%,%,$@))
CLEAN_MACRO__= $(MAKE) -C arch_$*_$(SO_NAME)/libname__ clean CROSS_ARCH=$* \
	INSTALL_DIR=$(INSTALL_DIR)
CLEAN_MACRO= $(CLEAN_MACRO__:libname__=$(patsubst $*_%_clean,%,$@))

# NOTE: 
# ’%_’ below is used to write target with wildcards (e.g. ‘%_’ matches all 
# targets that have that substring in ‘$(ARCHS:=_)’)
# ‘$*’ takes exactly the value of the substring matched by ‘%’ in the 
# correspondent target itself. 
%_: 
	make $*_ffmpeg_lhe
	make $*_unittest-cpp
	make $*_cjson
	make $*_uriparser
	make $*_mongoose
	make $*_sdl
	make $*_valgrind
	make $*_live555
	make $*_crc
	make $*_utils
	make $*_procs
#	make $*_transcoders
	make $*_codecs
	make $*_muxers
	make $*_examples

# To compile library "utils" standalone: "make x86_utils".
# To clean library "utils" standalone: "make x86_utils_clean".
# The same applies to the libraries below (e.g. "make x86_procs", 
# "make x86_codecs", ...)

%_ffmpeg_lhe:
	make $*_x264
	make $*_lame
	$(MAKE_MACRO)

%_x264:
	make $*_yasm
	make $*_nasm
	$(MAKE_MACRO)

%_utils \
%_procs \
%_transcoders \
%_codecs \
%_muxers \
%_lame \
%_cjson \
%_uriparser \
%_live555 \
%_crc:
	$(MAKE_MACRO)

%_examples \
%_yasm \
%_nasm \
%_unittest-cpp \
%_mongoose \
%_sdl \
%_valgrind:
	@if [[ $* == *86* ]]; then $(MAKE_MACRO); fi

clean: $(ARCHS:=_clean)

%_clean:
	make $*_ffmpeg_lhe_clean
	make $*_x264_clean
	make $*_lame_clean
	make $*_yasm_clean
	make $*_nasm_clean
	make $*_unittest-cpp_clean
	make $*_cjson_clean
	make $*_uriparser_clean
	make $*_live555_clean
	make $*_crc_clean
	make $*_mongoose_clean
	make $*_valgrind_clean
	make $*_sdl_clean
	make $*_utils_clean
	make $*_procs_clean
#	make $*_transcoders_clean
	make $*_codecs_clean
	make $*_muxers_clean
	make $*_examples_clean
	rm -rf *~ *.log *.log*
	rm -rf _install_dir_*

%_utils_clean \
%_procs_clean \
%_transcoders_clean \
%_codecs_clean \
%_muxers_clean \
%_ffmpeg_lhe_clean \
%_x264_clean \
%_lame_clean \
%_cjson_clean \
%_uriparser_clean \
%_live555_clean \
%_crc_clean:
	$(CLEAN_MACRO)

%_examples_clean \
%_yasm_clean \
%_nasm_clean \
%_unittest-cpp_clean \
%_mongoose_clean \
%_sdl_clean \
%_valgrind_clean:
	@if [[ $* == *86* ]]; then $(CLEAN_MACRO); fi;

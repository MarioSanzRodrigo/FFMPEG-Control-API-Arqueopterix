LIBNAME:= ffmpeg_lhe
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/LHE_c
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

# Export PATH to be able to find YASM in compilation
export PATH := $(PREFIX)/bin:$(PATH)

CONFIGURE_PARAMS= --prefix=${PREFIX}
CONFIGURE_PARAMS+= --disable-static --enable-shared
CONFIGURE_PARAMS+= --disable-nvenc
CONFIGURE_PARAMS+= --enable-gpl --enable-libx264
CONFIGURE_PARAMS+= --enable-libmp3lame
CONFIGURE_PARAMS+= --extra-cflags="-I${PREFIX}/include -fopenmp" \
	--extra-ldflags="-L${PREFIX}/lib -fopenmp"

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cd $(BUILD_DIR); \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install;

clean:
	@rm -fr $(BUILD_DIR)
	#@rm -f $(PREFIX)/bin/$(LIBNAME)*
	#@rm -rf $(PREFIX)/include/$(LIBNAME)*
	#@rm -f $(PREFIX)/lib/lib$(LIBNAME)*
	#@rm -rf $(PREFIX)/lib/pkgconfig/$(LIBNAME)*

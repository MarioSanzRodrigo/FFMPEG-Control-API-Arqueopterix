LIBNAME:= x264
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

# Export PATH to be able to find YASM in compilation
export PATH:= $(PREFIX)/bin:$(PATH)

CONFIGURE_PARAMS= --prefix=${PREFIX} --enable-shared --disable-avs --disable-swscale --disable-lavf --disable-ffms --disable-gpac --disable-lsmash

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(BUILD_DIR)/x264_config.h ] ; then \
		cd $(BUILD_DIR); \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install

clean:
	@rm -f  $(PREFIX)/bin/$(LIBNAME)
	@rm -f  $(PREFIX)/lib/lib$(LIBNAME)* $(PREFIX)/lib/pkgconfig/$(LIBNAME)*
	@rm -rf $(PREFIX)/include/$(LIBNAME)*
	@rm -rf $(BUILD_DIR)

LIBNAME:= lame
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=${PREFIX}

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
	cd $(BUILD_DIR); make install

clean:
	@rm -f  $(PREFIX)/bin/$(LIBNAME)
	@rm -f  $(PREFIX)/lib/libmp3$(LIBNAME)*
	@rm -rf $(PREFIX)/include/$(LIBNAME)
	@rm -rf $(PREFIX)/share/doc/$(LIBNAME)
	@rm -rf $(BUILD_DIR)

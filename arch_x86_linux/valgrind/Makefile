LIBNAME:= valgrind
BUILD_DIR := $(shell pwd)/build_dir
SOURCE_DIR := $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=$(PREFIX)

all: install

config:
	mkdir -p $(PREFIX)
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
	rm -fr $(BUILD_DIR) $(PREFIX)/include/$(LIBNAME) \
	$(PREFIX)/bin/$(LIBNAME)* $(PREFIX)/bin/callgrind* \
	$(PREFIX)/bin/cg_* $(PREFIX)/bin/ms_print \
	$(PREFIX)/bin/vgdb \
	$(PREFIX)/lib/pkgconfig/$(LIBNAME)* $(PREFIX)/lib/$(LIBNAME)

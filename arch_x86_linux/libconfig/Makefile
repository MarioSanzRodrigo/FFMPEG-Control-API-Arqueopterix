LIBNAME:= libconfig
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --srcdir=$(SOURCE_DIR) --prefix=$(PREFIX)

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(SOURCE_DIR)/configure ] ; then \
		cd $(SOURCE_DIR); autoreconf; \
	fi
	if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cd $(BUILD_DIR); \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install;

clean:
	@if [ -f $(BUILD_DIR)/Makefile ] ; then \
		cd $(BUILD_DIR); make maintainer-clean; \
	fi
	@rm -f $(SOURCE_DIR)/aclocal.m4
	@rm -f $(SOURCE_DIR)/configure
	@rm -fr $(BUILD_DIR)
	@rm -f $(PREFIX)/include/$(LIBNAME)*
	@rm -f $(PREFIX)/lib/$(LIBNAME)*
	@rm -f $(PREFIX)/lib/pkgconfig/$(LIBNAME)*
	@rm -f $(PREFIX)/share/info/$(LIBNAME)*

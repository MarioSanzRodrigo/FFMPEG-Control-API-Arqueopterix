LIBNAME:= uriparser
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=${PREFIX}
CONFIGURE_PARAMS+= --disable-static --disable-test --disable-wchar_t \
--disable-doc

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
	if [ ! -f $(LIB_DIR)/liburiparser.so ] ; then cd $(BUILD_DIR); make install; fi

clean:
	rm -rf $(BUILD_DIR)
	@rm -rf $(PREFIX)/lib/liburiparser* $(PREFIX)/lib/pkgconfig/liburiparser*
	@rm -rf $(PREFIX)/include/uriparser
	@rm -f $(PREFIX)/bin/uriparse

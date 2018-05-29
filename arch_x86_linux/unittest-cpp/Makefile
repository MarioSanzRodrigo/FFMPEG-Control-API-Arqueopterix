LIBNAME:= UnitTest++
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/unittest-cpp
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

export PATH := $(SOURCEDIR):$(PATH)

CONFIGURE_PARAMS= --prefix=$(PREFIX) --srcdir=$(SOURCE_DIR)

all: install

config:
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/lib
	@mkdir -p $(PREFIX)/include
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
                cd $(SOURCE_DIR); \
                ./autogen.sh; \
                cd $(BUILD_DIR); \
                $(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
        fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install;

clean:
	@#@-> do not echo commands on the output
	@rm -fr $(BUILD_DIR)
	@rm -rf $(PREFIX)/include/UnitTest++
	@rm -f $(PREFIX)/lib/lib$(LIBNAME)*
	@rm -rf $(PREFIX)/lib/pkgconfig/$(LIBNAME)*

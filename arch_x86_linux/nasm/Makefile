LIBNAME:= nasm
BUILD_DIR:= "$(shell pwd)"/build_dir
SOURCE_DIR:= "$(shell pwd)"/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=${PREFIX}

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cp -a $(SOURCE_DIR)/* $(BUILD_DIR); \
		cd $(BUILD_DIR); \
		./autogen.sh; \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install

clean:
	@rm -f  $(PREFIX)/bin/$(LIBNAME) $(PREFIX)/bin/ndisasm
	@rm -rf $(PREFIX)/share/man/man1/$(LIBNAME)* \
	$(PREFIX)/share/man/man1/ndisasm
	@rm -rf $(BUILD_DIR)

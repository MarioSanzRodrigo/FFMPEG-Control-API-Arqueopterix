LIBNAME:= yasm
BUILD_DIR:= "$(shell pwd)"/build_dir
SOURCE_DIR:= "$(shell pwd)"/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=${PREFIX} --enable-maintainer-mode

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(SOURCE_DIR)/configure ] ; then \
		cp ./autogen_yasm_modif.sh $(SOURCE_DIR); \
		$(SOURCE_DIR)/autogen_yasm_modif.sh; \
	fi
	if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cd $(BUILD_DIR); \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install

clean:
	@rm -f  $(PREFIX)/bin/*$(LIBNAME) $(PREFIX)/bin/ytasm
	@rm -f  $(PREFIX)/lib/lib$(LIBNAME)*
	@rm -rf $(PREFIX)/include/lib$(LIBNAME)*
	@rm -rf $(PREFIX)/share/man/man1/$(LIBNAME)*
	@rm -rf $(PREFIX)/share/man/man7/$(LIBNAME)*
	@rm -rf $(BUILD_DIR)


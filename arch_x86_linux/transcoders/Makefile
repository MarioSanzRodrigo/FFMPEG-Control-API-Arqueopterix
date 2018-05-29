LIBNAME:= mediaprocstranscoders
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../MediaProcessors/transcoders
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=$(PREFIX) --srcdir=$(SOURCE_DIR)

all: install

config:
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/lib
	@mkdir -p $(PREFIX)/include
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cd $(BUILD_DIR); \
		$(SOURCE_DIR)/configure $(CONFIGURE_PARAMS); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install
	#TODO: if flag GPL do TEST
	cd $(BUILD_DIR); make utest
	#endif

clean:
	@if [ -d $(BUILD_DIR) ] ; then \
		cd $(BUILD_DIR); make clean; \
	fi
	@rm -rf $(BUILD_DIR)

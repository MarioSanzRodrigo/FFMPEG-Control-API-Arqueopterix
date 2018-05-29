LIBNAME:= mediaprocscodecs
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../MediaProcessors/codecs
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

CONFIGURE_PARAMS= --prefix=$(PREFIX) --srcdir=$(SOURCE_DIR)

all: install

config:
	@mkdir -p $(PREFIX)
	@mkdir -p $(PREFIX)/lib
	@mkdir -p $(PREFIX)/include
	@mkdir -p $(BUILD_DIR)
	@(LD_LIBRARY_PATH=$(PREFIX)/lib $(PREFIX)/bin/ffmpeg -codecs > $(BUILD_DIR)/ffmpeg_codecs.txt)> /dev/null 2>/dev/null
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

LIBNAME:= live555
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

export CPPFLAGS := $(CPPFLAGS) -fPIC

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	if [ ! -f $(BUILD_DIR)/Makefile ] ; then \
		cp -a $(SOURCE_DIR)/* $(BUILD_DIR)/; \
		cd $(BUILD_DIR); \
		./genMakefiles $(SO_NAME); \
	fi

build: config
	cd $(BUILD_DIR); make

install: build
	cd $(BUILD_DIR); make install PREFIX=$(PREFIX)

clean:
	@rm -fr $(BUILD_DIR)

	@rm -f $(PREFIX)/lib/libBasicUsageEnvironment.a
	@rm -f $(PREFIX)/lib/libgroupsock.a
	@rm -f $(PREFIX)/lib/libliveMedia.a
	@rm -f $(PREFIX)/lib/libUsageEnvironment.a
	@rm -fr $(PREFIX)/include/BasicUsageEnvironment
	@rm -fr $(PREFIX)/include/groupsock
	@rm -fr $(PREFIX)/include/liveMedia
	@rm -fr $(PREFIX)/include/UsageEnvironment

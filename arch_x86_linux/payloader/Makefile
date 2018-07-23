LIBNAME:= payloader
BUILD_DIR:= "$(shell pwd)"/build_dir
SOURCE_DIR:= "$(shell pwd)"/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)
	mkdir -p $(SOURCE_DIR)/../../_install_dir_x86/include/libpayloader
	cd $(SOURCE_DIR); ./compile.sh;


install: config
	cd $(SOURCE_DIR); make;
	cp -R $(SOURCE_DIR)/CMakeFiles/prototype4_rec_rtsp.dir/src/libs $(BUILD_DIR);
	cp -R $(SOURCE_DIR)/CMakeFiles/prototype4_send_rtsp.dir/src/libs $(BUILD_DIR);
	cp -R $(SOURCE_DIR)/src/libs/*.h $(SOURCE_DIR)/../../_install_dir_x86/include/libpayloader;
	cp -R $(SOURCE_DIR)/src/libs/rtp/*.h $(SOURCE_DIR)/../../_install_dir_x86/include/libpayloader;
	

clean:
	rm -rf $(BUILD_DIR)

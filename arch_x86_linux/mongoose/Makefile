LIBNAME:= mongoose
BUILD_DIR:= $(shell pwd)/build_dir
SOURCE_DIR:= $(shell pwd)/../../3rdplibs/$(LIBNAME)
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	mkdir -p $(BUILD_DIR)

build: config
	if [ ! -f $(PREFIX)/lib/libmongoose.so ] ; then \
		cp -a $(SOURCE_DIR)/* $(BUILD_DIR)/; \
		cd $(BUILD_DIR)/examples; \
		$(CC) -c $(CFLAGS) -L$(PREFIX)/lib -I$(PREFIX)/include -Werror -fpic $(BUILD_DIR)/mongoose.c; \
		$(CC) -shared -Bdynamic $(CFLAGS) -o libmongoose.so mongoose.o; \
	fi

install: build
	cp $(BUILD_DIR)/examples/libmongoose.so $(PREFIX)/lib/;
	cp $(BUILD_DIR)/mongoose.h $(PREFIX)/include/;

clean:
	@rm -fr $(BUILD_DIR)
	@rm -f $(PREFIX)/lib/libmongoose.so
	@rm -f $(PREFIX)/include/mongoose.h

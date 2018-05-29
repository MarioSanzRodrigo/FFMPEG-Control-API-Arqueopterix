LIBNAME=cjson
BUILD_DIR := build_dir
SOURCE_DIR := $(shell pwd)/../../3rdplibs/cJSON
PREFIX:= $(INSTALL_DIR)$(CROSS_ARCH)
INCLUDE_DIR := "$(PREFIX)"/include
LIB_DIR := "$(PREFIX)"/lib

CFLAGS+=-I$(INCLUDE_DIR)

LIBS=-lm -ldl

_OBJ = $(wildcard $(SOURCE_DIR)/*.c*)
OBJ = $(patsubst $(SOURCE_DIR)/%.c*,$(BUILD_DIR)/%.o,$(_OBJ))

.PHONY : $(SOURCE_DIR)

all: install

config:
	mkdir -p $(PREFIX)
	mkdir -p $(INCLUDE_DIR)
	mkdir -p $(INCLUDE_DIR)/lib$(LIBNAME)
	mkdir -p $(LIB_DIR)
	mkdir -p $(BUILD_DIR)

build: config
	if [ ! -f $(LIB_DIR)/lib$(LIBNAME).so ] ; then \
	$(MAKE) $(BUILD_DIR)/lib$(LIBNAME).so; \
	fi

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c*
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/lib$(LIBNAME).so: $(OBJ)
	$(CC) -o $@ $^ -shared -Bdynamic $(CFLAGS) $(LIBS)

install: build
	cp $(BUILD_DIR)/*.so $(LIB_DIR)/;
	cp -a $(SOURCE_DIR)/*.h $(INCLUDE_DIR)/lib$(LIBNAME)/;

clean:
	rm -rf *~ *.log *.log* $(SOURCE_DIR)/*~ $(BUILD_DIR) \
	$(LIB_DIR)/lib$(LIBNAME)* $(INCLUDE_DIR)/lib$(LIBNAME)

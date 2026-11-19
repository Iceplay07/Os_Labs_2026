
CC = cc
CFLAGS = -Wall -Wextra -Iinclude
OS = $(shell uname -s)

ifeq ($(OS),Darwin)
    EXT = dylib
    SHARED = -dynamiclib -Wl,-install_name,@rpath/$(notdir $@)
    RPATH = -Wl,-rpath,@loader_path
    DL =
else
    EXT = so
    SHARED = -shared
    RPATH = -Wl,-rpath,'$$ORIGIN'
    DL = -ldl
endif

all: build/libimpl1.$(EXT) build/libimpl2.$(EXT) build/program1 build/program2

build:
	mkdir -p build

build/libimpl%.$(EXT): src/impl%.c include/library.h | build
	$(CC) $(CFLAGS) -fPIC $(SHARED) -o $@ $< -lm

build/program1: src/program1.c include/library.h build/libimpl1.$(EXT)
	$(CC) $(CFLAGS) -o $@ $< -Lbuild -limpl1 $(RPATH) -lm

build/program2: src/program2.c include/library.h | build
	$(CC) $(CFLAGS) -o $@ $< $(DL) -lm

run1: all
	cd build && ./program1

run2: all
	cd build && ./program2

test: all
	python3 tests/test_lab.py

clean:
	rm -rf build

.PHONY: all run1 run2 test clean

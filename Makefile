.PHONY: all clean

BUILD_VERSION ?= Release

all:
	cd scripts; ./setup; ./build ${BUILD_VERSION}

install:
	cd scripts; ./install;

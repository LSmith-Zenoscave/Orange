.PHONY: all clean

all:
	cd scripts; ./setup; ./build Release

clean:
	cd scripts; ./clean;

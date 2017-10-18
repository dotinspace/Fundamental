
SOURCES = $(wildcard src/*.cpp)

include ../../plugin.mk


dist: all
	mkdir -p dist/MrLumps
	cp LICENSE* dist/MrLumps/
	cp plugin.* dist/MrLumps/
	cp -R res dist/MrLumps/

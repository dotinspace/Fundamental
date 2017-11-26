
SOURCES = $(wildcard src/*.cpp)

# Must include the VCV plugin Makefile framework
include ../../plugin.mk

# Convenience target for including files in the distributable release
DIST_NAME = MrLumps
.PHONY: dist
dist: all
ifndef VERSION
	$(error VERSION must be defined when making distributables)
endif
	mkdir -p dist/$(DIST_NAME)
	cp LICENSE* dist/$(DIST_NAME)/
	cp $(TARGET) dist/$(DIST_NAME)/
	cp -R res dist/$(DIST_NAME)/
	cd dist && zip -5 -r $(DIST_NAME)-$(VERSION)-$(ARCH).zip $(DIST_NAME)

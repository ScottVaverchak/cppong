CXX = clang++
PKGS=sdl2 sdl2_ttf
CXXFLAGS=-Wall -pedantic -std=c++17 -ggdb -stdlib=libc++ $(shell pkg-config --cflags $(PKGS))
LIBS=$(shell pkg-config --libs $(PKGS))
OUTPUT=build build/assets
ASSETS=assets

cppong: main.cpp
	rm -rf $(OUTPUT)
	mkdir -p $(OUTPUT)
	cp -r $(ASSETS) $(OUTPUT)/$(ASSETS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT)/cppong src/macos_cppong.cpp $(LIBS)

CXX = clang++
PKGS=sdl2 sdl2_ttf libpng
CXXFLAGS=-Wall -pedantic -std=c++17 -ggdb -stdlib=libc++ $(shell pkg-config --cflags $(PKGS))
LIBS=$(shell pkg-config --libs $(PKGS))
OUTPUT=build

cppong: main.cpp
	$(shell mkdir -p $(OUTPUT))
	$(CXX) $(CXXFLAGS) -o $(OUTPUT)/cppong macos_cppong.cpp $(LIBS)



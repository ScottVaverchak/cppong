CXX = clang++
# SDL = -framework SDL2
PKGS=sdl2
CXXFLAGS=-Wall -pedantic -std=c++17 -ggdb -stdlib=libc++ $(shell pkg-config --cflags $(PKGS))
LIBS=$(shell pkg-config --libs $(PKGS))

cppong: main.cpp
	$(CXX) $(CXXFLAGS) -o cppong main.cpp $(LIBS)



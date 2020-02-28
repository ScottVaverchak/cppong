CXX = clang++
# SDL = -framework SDL2
SDL = -I/usr/local/include -L/usr/local/lib -lSDL2
CXXFLAGS = -Wall -c -std=c++17 -stdlib=libc++
LDFLAGS = $(SDL)
EXE = cppong

all: $(EXE)

$(EXE): main.o
	$(CXX) $(LDFLAGS) $< -o $@

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)

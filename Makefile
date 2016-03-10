### Makefile for 2048 ###

# Change to desired executable name
EXECUTABLE = 2048
# Change to desired compiler
CXX = g++
SDL_C = `sdl2-config --cflags`
SDL_L = `sdl2-config --libs` -lSDL2_ttf -lSDL2_image

TESTSOURCES = $(wildcard *test*.cpp)
HEADERS = $(wildcard *.h)
SOURCES = $(wildcard *.cpp)
SOURCES     := $(filter-out $(TESTSOURCES), $(SOURCES))
OBJECTS		= $(SOURCES:%.cpp=%.o)

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Wvla -Wconversion -Wextra -pedantic

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#				 also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: all

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG so that "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

# highest target; sews together all objects into executable
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
ifeq ($(EXECUTABLE), executable)
	@echo Using default a.out.
	$(CXX) $(CXXFLAGS) $(SDL_L) $(OBJECTS)
else
	$(CXX) $(CXXFLAGS) $(SDL_L) $(OBJECTS) -o $(EXECUTABLE)
endif

$(OBJECTS): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SDL_C) $(SOURCES) -c

# make clean - remove .o files, executables, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TESTS)

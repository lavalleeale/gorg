# Compiler
CXX = g++

# Precompiled headers (add as many as needed)
PCH_LIST = include/exprtk.hpp include/nlohmann/json.hpp
GCH_LIST = $(PCH_LIST:=.gch)

# Use pkg-config to get necessary flags for gtkmm-3.0 and libcurl
PKG_CFLAGS = $(shell pkg-config gtkmm-3.0 libcurl --cflags)
PKG_LIBS   = $(shell pkg-config gtkmm-3.0 libcurl --libs)

# Compilation flags: C++17 standard, include directory and pkg-config flags
CXXFLAGS = -std=c++17 -Iinclude $(PKG_CFLAGS)

# Linker flags
LDFLAGS = $(PKG_LIBS)

# Gather all .cpp files and convert them to object file names
SOURCES = $(wildcard **/*.cpp) $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable name
TARGET = launcher.out 

# Default target
all: $(GCH_LIST) $(TARGET)

# Pattern rule for building precompiled headers
%.gch: %
	$(CXX) $(CXXFLAGS) -x c++-header $< -o $@

# Link the object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile each source file into an object file; depend on all precompiled headers
%.o: %.cpp $(GCH_LIST)
	$(CXX) $(CXXFLAGS) $(foreach pch,$(PCH_LIST),-include $(pch)) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET) $(GCH_LIST)

# Install target for use with Nix
install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/launcher

.PHONY: debug clean install
debug: CXXFLAGS += -g -O0
debug: clean all
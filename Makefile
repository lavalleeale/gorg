# Compiler
CXX = g++

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
TARGET = gorg.out 

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile each source file into an object file; depend on all precompiled headers
%.o: %.cpp $(GCH_LIST)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install target for use with Nix
install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/gorg

.PHONY: debug clean install
debug: CXXFLAGS += -g -O0
debug: clean all
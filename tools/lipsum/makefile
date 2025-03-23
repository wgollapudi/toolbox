CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

PREFIX   ?= /usr/local
BINDIR   := $(PREFIX)/bin

SOURCES  := src/main.cpp src/grammar.cpp src/lexicon.cpp src/dictionary.cpp
OBJECTS  := $(patsubst src/%.cpp,%.o,$(SOURCES))

TARGET   := lipsum

.PHONY : all clean install uninstall

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	rm -f $(OBJECTS) $(TARGET)

install : $(TARGET)
	@echo "Installing $(TARGET) to $(BINDIR)"
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)

uninstall :
	@echo "Uninstalling $(TARGET) from $(BINDIR)"
	rm -f $(BINDIR)/$(TARGET)


CC = g++
CFLAGS = -Wall -std=c++17
LDFLAGS =

SOURCES_CPP = main.cpp
SOURCES_C = SegaPVRImage.c
OBJECTS = $(SOURCES_CPP:.cpp=.o) $(SOURCES_C:.c=.o)
EXECUTABLE = spvr2png

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
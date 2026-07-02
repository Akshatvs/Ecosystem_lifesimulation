CC = gcc

CFLAGS = -Wall -Wextra -Iinclude
LIBS = $(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)

SRC = src/main.c \
      src/simulation.c \
      src/rendering.c \
      src/pixel_art.c

TARGET = ecosystem-simulation

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
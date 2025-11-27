SOURCES := $(wildcard src/*.c)
TARGET  := imgprocc
CFLAGS  := -Wall -Wextra
LDFLAGS := -lm

$(TARGET): $(SOURCES)
	@gcc $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	@./$(TARGET)

debug: $(SOURCES)
	@gcc $(CFLAGS) -g $(LDFLAGS) $(SOURCES) -o $(TARGET)
	@gdb ./$(TARGET)

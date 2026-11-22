CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
CPPFLAGS += -Iinclude
LDLIBS += -lm

TARGET := rotas
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@ $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

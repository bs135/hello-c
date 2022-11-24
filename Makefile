# app name
PROGRAM = hello

# program source files
SOURCE_FILES = $(shell find src -type f -name '*.c')

# object files
OBJECT_FILES = $(shell find ./ -type f -name '*.o')

# MAC OS temp files
MAC_TEMP_FILES = $(shell find ./ -type f -name '._*')

# collect all .cc source files
SOURCES = $(SOURCE_FILES)

# object files
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

# lib link flags
LDFLAGS = -luuid -ljson-c -lpthread

# target flags
CXXFLAGS = -O0 -std=c11 -Wall -Wextra -fcommon #-ggdb

# include -I
CFLAGS = -Isrc -Ilib

# defines
CFLAGS += -DVERSION=\"${VERSION}\"
CFLAGS += -DLOG_USE_COLOR -DLOG_USE_SYSLOG

.PHONY: $(PROGRAM) clean

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(CXXFLAGS) $(OBJECTS) -o $(PROGRAM) $(LDFLAGS)

clean:
	@- $(RM) $(PROGRAM)
	@- $(RM) $(OBJECT_FILES)
	@- $(RM) $(MAC_TEMP_FILES)

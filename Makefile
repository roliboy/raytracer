TARGET = tracer

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(shell find $(SRCDIR) -name *.c)
INCLUDES := $(shell find $(SRCDIR) -name *.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = tcc
CFLAGS = -Wall -Wpedantic -mavx2 -I$(SRCDIR)
# CFLAGS = -std=c99 -Wall -I.

LINKER = gcc
LFLAGS = -lm -lSDL2
# LFLAGS   = -Wall -I. -lm

$(BINDIR)/$(TARGET): directories $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully"

.PHONY: directories
directories:
	@mkdir -p $(patsubst src%, obj%, $(shell find $(SRCDIR) -type d))

.PHONY: clean
clean:
	@rm -f $(OBJECTS)
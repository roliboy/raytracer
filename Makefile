TARGET = tracer

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES := $(shell find $(SRCDIR) -name *.c)
INCLUDES := $(shell find $(SRCDIR) -name *.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC = gcc
CFLAGS = -g -Wall -Wpedantic -mavx2 -fopenmp -I$(SRCDIR)
# CFLAGS = -std=c99 -Wall -I.

LINKER = gcc
LFLAGS = -g -lm -lSDL2 -fopenmp
# LFLAGS   = -Wall -I. -lm

$(BINDIR)/$(TARGET): directories $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully"

.PHONY: directories
directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(patsubst src%, obj%, $(shell find $(SRCDIR) -type d))

.PHONY: run
run:
	@$(BINDIR)/$(TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS)

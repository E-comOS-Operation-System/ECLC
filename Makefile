# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
LDFLAGS = 
LDLIBS = 

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files - organized by module
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/common/men.c \
          $(SRCDIR)/driver/args.c \
          $(SRCDIR)/frontend/lexer.c \
          $(SRCDIR)/frontend/parser.c \
          $(SRCDIR)/frontend/ast.c \
          $(SRCDIR)/frontend/error.c \
          $(SRCDIR)/fcef/fcef.c

# FCEF-specific sources (for ECLC to FCEF conversion)
FCEF_SOURCES = $(SRCDIR)/fcef/eclc_fcef.c

# Combine all sources
ALL_SOURCES = $(SOURCES) $(FCEF_SOURCES)

# Object files
OBJECTS = $(ALL_SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Targets
TARGET = $(BINDIR)/eclc
FCEF_TEST = $(BINDIR)/fcef_test

.PHONY: all clean fcef-test

all: $(TARGET)

# Main ECLC compiler
$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

# Object file compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# FCEF test executable
fcef-test: $(FCEF_TEST)

$(FCEF_TEST): $(OBJDIR)/fcef/fcef.o $(OBJDIR)/fcef/eclc_fcef.o | $(BINDIR)
	$(CC) $^ -o $@

# Directory creation
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Install (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/eclc

# Debug info
debug-info:
	@echo "Sources: $(ALL_SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Target: $(TARGET)"
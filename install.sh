#!/bin/bash
# ECLC Installation Script

set -e

echo "Installing ECLC - E-comOS C/C++ Language Compiler..."

# Build the compiler
echo "Building ECLC..."
make clean
make

# Create installation directory
INSTALL_DIR="/usr/local/bin"
if [ ! -w "$INSTALL_DIR" ]; then
    INSTALL_DIR="$HOME/.local/bin"
    mkdir -p "$INSTALL_DIR"
fi

# Install binary
echo "Installing to $INSTALL_DIR..."
cp bin/eclc "$INSTALL_DIR/"
chmod +x "$INSTALL_DIR/eclc"

# Add to PATH if needed
if [[ ":$PATH:" != *":$INSTALL_DIR:"* ]]; then
    echo "Add $INSTALL_DIR to your PATH:"
    echo "export PATH=\"$INSTALL_DIR:\$PATH\""
fi

echo "✅ ECLC installed successfully!"
echo "Usage: eclc <file.c> [-o output] | eclc -f <folder>"
#!/bin/bash
# Flow Language - Quick Install Script

set -e

echo "================================"
echo "Flow Language Installer"
echo "================================"
echo ""

# Check for g++
if ! command -v g++ &> /dev/null; then
    echo "❌ g++ not found. Please install g++ first."
    exit 1
fi

echo "✓ g++ found"

# Check for Python
if ! command -v python3 &> /dev/null && ! command -v python &> /dev/null; then
    echo "❌ Python not found. Please install Python 3.x first."
    exit 1
fi

echo "✓ Python found"

# Check for Node.js
if ! command -v node &> /dev/null; then
    echo "❌ Node.js not found. Please install Node.js first."
    exit 1
fi

echo "✓ Node.js found"

echo ""
echo "Building Flow..."

# Compile Flow
g++ -std=c++17 -O2 -o flow src/flow.cpp

if [ $? -eq 0 ]; then
    echo "✓ Flow compiled successfully"
else
    echo "❌ Compilation failed"
    exit 1
fi

echo ""
echo "Testing Flow..."

# Run basic test
./flow examples/test.fl > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "✓ Tests passed"
else
    echo "❌ Tests failed"
    exit 1
fi

echo ""
echo "================================"
echo "Installation Complete!"
echo "================================"
echo ""
echo "Flow is ready to use:"
echo "  ./flow examples/test.fl"
echo ""
echo "To install system-wide:"
echo "  sudo make install"
echo ""
echo "Documentation:"
echo "  README.md"
echo "  docs/QUICKSTART.md"
echo ""
echo "Happy coding! 🚀"

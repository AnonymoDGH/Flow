# Flow Language Makefile

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET = flow
SRC = src/flow.cpp

# Platform detection
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM = del /Q
    MKDIR = mkdir
else
    RM = rm -f
    MKDIR = mkdir -p
endif

.PHONY: all clean install test examples

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "Compiling Flow..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
	@echo "✓ Flow compiled successfully"

clean:
	@echo "Cleaning..."
	$(RM) $(TARGET) __flow__* __cleanup__* __flow_bin__* 2>nul || true
	@echo "✓ Cleaned"

install: $(TARGET)
	@echo "Installing Flow..."
ifeq ($(OS),Windows_NT)
	@echo "Copy flow.exe to a directory in your PATH"
	@echo "Example: copy flow.exe C:\Windows\System32\"
else
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	@echo "✓ Flow installed to /usr/local/bin/"
endif

test: $(TARGET)
	@echo "Running tests..."
	./$(TARGET) examples/test.fl
	./$(TARGET) examples/memory_test.fl
	./$(TARGET) examples/multi_file_test.fl
	@echo "✓ All tests passed"

examples: $(TARGET)
	@echo "Running examples..."
	./$(TARGET) examples/advanced_demo.fl
	@echo "✓ Examples completed"

help:
	@echo "Flow Language Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Compile Flow"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make install  - Install Flow system-wide"
	@echo "  make test     - Run test suite"
	@echo "  make examples - Run example programs"
	@echo "  make help     - Show this help"

# Compiler
GPP = g++
GPPFL = -Wall -Wextra -std=c++17

# Directories
SRC = Client
BUILD = build
BIN = bin

# Find all .cpp files and corresponding .o files
SRCS := $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, $(BUILD)/%.o, $(SRCS))

# Output binary
TARGET = $(BIN)/redis_cli

# Default target
all: $(TARGET)

# Create directories if not there
$(BUILD) $(BIN):
	mkdir -p $@

# Compile source files to object files
$(BUILD)/%.o: $(SRC)/%.cpp | $(BUILD)
	$(GPP) $(GPPFL) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJS) | $(BIN)
	$(GPP) $(GPPFL) $(OBJS) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD) $(BIN)

# Rebuild from scratch
rebuild: clean all

# Run the compiled program
run: all
	./$(TARGET)

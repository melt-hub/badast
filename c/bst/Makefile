
# -Iinclude tells make where to look for header files
CFLAGS = -O2 -Wall -Wextra -Iinclude
CC = gcc

# Directories
SRC = src
TEST = tests
INC = include/bst
BUILD = build
BIN = bin

# Binary file name
TARGET = $(BIN)/testunit

# Main target
all: directories $(TARGET)

# ----------+ LINKING +----------
$(TARGET): $(BUILD)/testunit.o $(BUILD)/bst.o
	@echo ">>> Linking and assembling in $(BUILD)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(BUILD)/testunit.o $(BUILD)/bst.o

# ----------+ COMPILATION +----------
$(BUILD)/bst.o : $(SRC)/bst.c $(INC)/bst.h
	@echo ">>> Compiling bst.c..."
	$(CC) $(CFLAGS) -c $(SRC)/bst.c -o $(BUILD)/bst.o

$(BUILD)/testunit.o : $(TEST)/testunit.c $(INC)/testbst.h $(INC)/bst.h
	@echo ">>> Compiling testunit.c..."
	$(CC) $(CFLAGS) -c $(TEST)/testunit.c -o $(BUILD)/testunit.o

# ----------+ DIRECTORIES SETUP +----------
# (mkdir -p does not return an error if directories already exists)
directories:
	@mkdir -p $(BUILD) $(BIN)

# ----------+ CLEAN UP +----------
clean:
	@echo ">>> Cleaning up..."
	rm -f $(BUILD)/*.o $(TARGET)

# ----------+ QUICK RUN +----------
# Create / Basic
runc: $(TARGET)
	./$(TARGET) c

# Insert
runi: $(TARGET)
	./$(TARGET) i

# Delete
rund: $(TARGET)
	./$(TARGET) d

# Search / Traversal
runs: $(TARGET)
	./$(TARGET) s

# Metrics
runm: $(TARGET)
	./$(TARGET) m

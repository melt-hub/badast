
# -Iinclude tells make where to look for header files
CFLAGS = -O2 -Wall -Wextra -Iinclude
CC = gcc

# Directories
SRC = src
TEST = tests
INC = include/vector
BUILD = build
BIN = bin

# Binary file name
TARGET = $(BIN)/testunit

# Main target
all: directories $(TARGET)

# ----------+ LINKING +----------
$(TARGET): $(BUILD)/testunit.o $(BUILD)/vector.o
	@echo ">>> Linking and sborring in $(BUILD)..."
	$(CC) $(CFLAGS) -o $(TARGET) $(BUILD)/testunit.o $(BUILD)/vector.o

# ----------+ COMPILATION +----------
$(BUILD)/vector.o : $(SRC)/vector.c $(INC)/vector.h
	@echo ">>> Compiling vector.c..."
	$(CC) $(CFLAGS) -c $(SRC)/vector.c -o $(BUILD)/vector.o

$(BUILD)/testunit.o : $(TEST)/testunit.c $(INC)/testvector.h $(INC)/vector.h
	@echo ">>> Compiling testunit.c"
	$(CC) $(CFLAGS) -c $(TEST)/testunit.c -o $(BUILD)/testunit.o

# ----------+ DIRECTORIES SETUP +----------
# (mkdir -p does not return an error if directories already exists)
directories:
	@mkdir -p $(BUILD) $(BIN)

# ----------+ CLEAN UP +----------
clean:
	@echo ">>> Cleaning up..."
	rm -f $(BUILD)/*.o testunit $(TARGET)

# ----------+ QUICK RUN +----------
runc: $(TARGET)
	./$(TARGET) c

runi: $(TARGET)
	./$(TARGET) i

rund: $(TARGET)
	./$(TARGET) d

runs: $(TARGET)
	./$(TARGET) s


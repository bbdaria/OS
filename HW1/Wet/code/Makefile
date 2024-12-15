SUBMITTERS := 328137831_327900817

# Variables
COMPILER := g++
CODE_DIR := .  # Start from the current directory
INNER_DIRECTORIES := $(shell find $(CODE_DIR) -type d -exec realpath --relative-to=$(CODE_DIR) {} \;)
COMPILER_FLAGS := --std=c++11 -Wall
SMASH_BIN := smash

# Find all source files recursively under the CODE_DIR and inner directories
SRCS := $(foreach dir,$(INNER_DIRECTORIES),$(wildcard $(dir)/*.cpp))

# Find all header files recursively under the CODE_DIR and inner directories
HDRS := $(foreach dir,$(INNER_DIRECTORIES),$(wildcard $(dir)/*.h))

# Object files
OBJS := $(SRCS:.cpp=.o)

# Add -I flag for each directory found
COMPILER_FLAGS += $(foreach dir,$(INNER_DIRECTORIES), -I$(dir))

# Targets
.PHONY: all clean

# Main target
all: $(SMASH_BIN)

# Rule to build the smash executable
$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

# Rule to build object files
$(OBJS): %.o: %.cpp $(HDRS)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(SMASH_BIN)

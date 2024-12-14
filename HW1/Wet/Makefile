SUBMITTERS := 328137831_327900817

# Variables
COMPILER := g++
COMPILER_FLAGS := --std=c++11 -Wall -Icode
SRC_DIR := code
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)
HDRS := $(shell find $(SRC_DIR) -name '*.h')
SMASH_BIN := smash

# Targets
.PHONY: smash clean

# Main target
smash: $(SMASH_BIN)

# Rule to build the smash executable
$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

# Rule to build object files
$(OBJS): %.o: %.cpp $(HDRS)
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(SMASH_BIN)

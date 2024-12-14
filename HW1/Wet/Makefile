# TODO: replace ID with your own IDS, for example: 123456789_123456789
SUBMITTERS := 328137831_327900817
COMPILER := g++
COMPILER_FLAGS := --std=c++11 -Wall -Icode
CODE_DIR := code
SRCS := $(CODE_DIR)/main.cpp $(CODE_DIR)/util.cpp $(CODE_DIR)/signals/signals.cpp $(CODE_DIR)/smash/smash.cpp
OBJS := $(SRCS:.cpp=.o)
HDRS := $(shell find $(CODE_DIR) -name '*.h')  # Dynamically find all .h files
TESTS_INPUTS := $(wildcard tests/test_input*.txt)
TESTS_OUTPUTS := $(subst input,output,$(TESTS_INPUTS))
SMASH_BIN := smash

.PHONY: all clean test zip

all: $(SMASH_BIN)

test: $(TESTS_OUTPUTS)
	@echo "All tests completed."

$(TESTS_OUTPUTS): $(SMASH_BIN)
$(TESTS_OUTPUTS): tests/test_output%.txt: tests/test_input%.txt tests/test_expected_output%.txt
	@./$(SMASH_BIN) < $(word 1, $^) > $@
	@test -s $@ && test -s $(word 2, $^) || (echo "Error: Empty file in test comparison"; exit 1)
	@if diff -q $@ $(word 2, $^); then \
	    echo -e "\e[32m$(word 1, $^) ++PASSED++\e[0m"; \
	else \
	    echo -e "\e[31m$(word 1, $^) --FAILED--\e[0m"; \
	    diff $@ $(word 2, $^); \
	fi

$(SMASH_BIN): $(OBJS)
	$(COMPILER) $(COMPILER_FLAGS) $^ -o $@

$(OBJS): %.o: %.cpp
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

zip: $(SRCS) $(HDRS)
	zip -j $(SUBMITTERS).zip $(SRCS) $(HDRS) submitters.txt Makefile

clean:
	rm -f $(SMASH_BIN) $(OBJS) $(TESTS_OUTPUTS)
	rm -f $(SUBMITTERS).zip

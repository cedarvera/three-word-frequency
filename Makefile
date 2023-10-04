BIN = main

CXXFLAGS = -g -Wall

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))
TEST_SRC := $(wildcard test/*.cpp)
TEST_OBJ := $(filter-out src/main.o,$(OBJ)) $(patsubst %.cpp,%.o,$(TEST_SRC))

.DEFAULT: all
all: build

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# get the files we want
texts:
	@mkdir -p texts
	@wget -O texts/pg2600.txt https://gutenberg.org/cache/epub/2600/pg2600.txt
	@wget -O texts/pg2701.txt https://gutenberg.org/cache/epub/2701/pg2701.txt

# NOTE: One off but want to keep a record of where it came from.
#         Copy dictionary from my computer
#         not necessary availabe to anyone so let's
#         add it to the code base.
.PHONY: dict
dict:
	@cat /usr/share/dict/words | awk '{ print tolower($0) }' | sort -u > dict/words

.PHONY: setup
setup: texts

.PHONY: build
build: setup $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(BIN)

.PHONY: test
test: $(TEST_OBJ)
	@$(CXX) $(CXXFLAGS) -g $(TEST_OBJ) -o $(BIN) -lCppUTest

.PHONY: clean
clean:
	@$(RM) $(BIN)
	@$(RM) -r $(OBJ)
	@$(RM) -r $(TEST_OBJ)

.PHONY: dist-clean
dist-clean: clean
	@$(RM) -r texts

# for ease of use
.PHONY: run-test
run-test: test
	@./$(BIN)

.PHONY: run-stdin
run-stdin: build
	@cat texts/*.txt | ./$(BIN)

.PHONY: run-file
run-file: build
	@./$(BIN) "texts/pg2600.txt" "texts/pg2701.txt"

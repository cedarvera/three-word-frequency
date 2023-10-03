BIN = main

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst %.cpp,%.o,$(SRC))

.DEFAULT: all
all: setup build

%.o: %.cpp
	$(CXX) -c $< -o $@

# get the files we want
texts:
	mkdir -p texts
	wget -O texts/pg2000.txt https://gutenberg.org/cache/epub/2600/pg2600.txt
	wget -O texts/pg2701.txt https://gutenberg.org/cache/epub/2701/pg2701.txt

.PHONY: setup
setup: texts

.PHONY: build
build: $(OBJ)
	$(CXX) $< -o $(BIN)

.PHONY: clean
clean:
	$(RM) $(BIN)
	$(RM) -r $(shell find -name *.o)

.PHONY: dist-clean
dist-clean: clean
	$(RM) -r texts

# for ease of use
.PHONY: run-stdin
run-stdin: build
	cat texts/*.txt | ./$(BIN)

.PHONY: run-file
run-file: build
	./$(BIN) texts/pg2600.txt texts/pg2701.txt

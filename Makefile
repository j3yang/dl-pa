TARGET_EXEC := dlpa

LEX = flex
LFLAGS = -o
YACC = bison
YFLAGS = --verbose --debug -d
CXXFLAGS = -std=c++17

SRC_DIRS := ./src
BUILD_DIR := ./build

$(shell mkdir -p $(BUILD_DIR))

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

$(BUILD_DIR)/$(TARGET_EXEC): $(SRCS) $(BUILD_DIR)/lex.yy.o $(SRC_DIRS)/Parser.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS) $(BUILD_DIR)/lex.yy.o $(SRC_DIRS)/Parser.cpp -o $@

$(BUILD_DIR)/lex.yy.o: $(SRC_DIRS)/lex.yy.c $(SRC_DIRS)/Parser.cpp
	$(CC) -c $(SRC_DIRS)/lex.yy.c -o $@

$(SRC_DIRS)/lex.yy.c: $(SRC_DIRS)/Lexer.l
	$(LEX) $(LFLAGS) $@ $(SRC_DIRS)/Lexer.l

$(SRC_DIRS)/Parser.cpp: $(SRC_DIRS)/Parser.y
	$(YACC) $(YFLAGS) $(SRC_DIRS)/Parser.y -o $@

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/* $(SRC_DIRS)/Parser.output $(SRC_DIRS)/Parser.cpp $(SRC_DIRS)/Parser.hpp $(SRC_DIRS)/lex.yy.c 

# flex Lexer.l &&
#  bison --verbose --debug -d Parser.y -o Parser.cpp &&
#   gcc -c lex.yy.c -o lex.yy.o &&
#    g++ -std=c++17 Algorithms.cpp Utility.cpp Parameters.cpp FormulaGenerator.cpp Formula.cpp Program.cpp lex.yy.o Parser.cpp -o dlpa


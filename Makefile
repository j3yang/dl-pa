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
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/Parser.o
	$(CXX) $(OBJS) $(BUILD_DIR)/lex.yy.o $(BUILD_DIR)/Parser.o -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/lex.yy.o: $(SRC_DIRS)/lex.yy.c $(SRC_DIRS)/Parser.cpp
	$(CC) -c $< -o $@

$(SRC_DIRS)/lex.yy.c: $(SRC_DIRS)/Lexer.l
	$(LEX) $(LFLAGS) $@ $<

$(BUILD_DIR)/Parser.o: $(SRC_DIRS)/Parser.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(SRC_DIRS)/Parser.cpp: $(SRC_DIRS)/Parser.y
	$(YACC) $(YFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/* $(SRC_DIRS)/Parser.output $(SRC_DIRS)/Parser.cpp $(SRC_DIRS)/Parser.hpp $(SRC_DIRS)/lex.yy.c 

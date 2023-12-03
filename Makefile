#compiler setup
CXX=g++
CC=g++
LD=g++
CXXFLAGS=-g

#targets
TARGETS=lexer_test parser_test calc scope_test

all: $(TARGETS)
lexer_test: lexer_test.o lexer.o
parser_test: parser.o lexer.o parser_test.o parse_tree.o ref_env.o
calc: parser.o lexer.o calc.o parse_tree.o ref_env.o
scope_test: parser.o lexer.o scope_test.o parse_tree.o ref_env.o


clean:
	rm -f *.o $(TARGETS)
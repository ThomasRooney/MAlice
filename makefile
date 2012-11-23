CC=g++
ANTLR_SEARCH_PATHS=-I grammar/antlr/include -I grammar/antlr/libantlr3c
LEXER_SEARCH_PATH=-I grammar/output
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
H_FILES = $(wildcard src/*.h)
OBJECTS = ${CPP_FILES:.cpp=.o}
CCFLAGS = -std=c++0x


all:  compile
	
obj/%.o: src/%.cpp obj/MAliceLexer.o obj/MAliceParser.o grammar/output/MAliceLexer.c grammar/output/MAliceParser.c
	$(CC) $(CCFLAGS) $(ANTLR_SEARCH_PATHS) $(LEXER_SEARCH_PATH) grammar/antlr/libantlr3c/.libs/libantlr3c.a obj/MAliceLexer.o obj/MAliceParser.o -o $@ $<

compile: $(OBJ_FILES) $(ANTLR_OUTPUT_FILES) obj/MAliceLexer.o obj/MAliceParser.o antlr
	echo $(OBJ_FILES)
	$(CC) $(CCFLAGS) $(OBJ_FILES) obj/MAliceLexer.o obj/MAliceParser.o grammar/antlr/libantlr3c/.libs/libantlr3c.a -o $@

# Building the lexer and parser generated by ANTLR
antlr: 
	sh ./configureantlr.sh

obj/MAliceLexer.o: antlr
	$(CC) $(ANTLR_SEARCH_PATHS) -c grammar/output/MAliceLexer.c -o obj/MAliceLexer.o

obj/MAliceParser.o: antlr
	$(CC) $(ANTLR_SEARCH_PATHS) -c grammar/output/MAliceParser.c -o obj/MAliceParser.o



clean:
	@sh ./cleanantlr.sh
	rm obj/*.o
.PHONY: clean

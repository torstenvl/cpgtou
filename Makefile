ifeq ($(OS),Windows_NT)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        REL_FLG = -std=c2x -Os
        DBG_FLG = -std=c2x -O0 -pg
        STR_FLG = -W -Wall -Werror -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith \
		          -Wreturn-type -Wcast-qual -Wswitch -Wshadow -Wcast-align -Wwrite-strings \
				  -Wmisleading-indentation -Wno-parentheses -Wno-unused-function -Wno-unused-value
        VSTR_FLG = -Wunused-parameter -Wchar-subscripts -Winline -Wnested-externs -Wredundant-decls
        XSTR_FLG = -Wextra 
    endif
    ifeq ($(UNAME_S),Darwin)
        REL_FLG  =  -std=c2x -Oz
        DBG_FLG  =  -std=c2x -O0 -gfull
        STR_FLG  =  -W -Wall -Werror -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith \
		            -Wreturn-type -Wcast-qual -Wswitch -Wshadow -Wcast-align -Wwrite-strings \
				    -Wmisleading-indentation -Wno-bitwise-op-parentheses -Wno-unused-function
        VSTR_FLG =  -Wunused-parameter -Wchar-subscripts -Winline -Wnested-externs -Wredundant-decls
        XSTR_FLG =  -Weverything -Wno-gnu-binary-literal -Wno-poison-system-directories \
		            -Wno-padded -Wno-c99-compat
    endif
endif




SHELL:=/bin/bash

OBJ=cpgtou.o
SRC=cpgtou.h

SUCC="\342\234\205 SUCCESS\n"
FAIL="\342\235\214\033[1;31m FAILED!!!\033[m\n"

CC = cc

.PHONY: all
all: module

.PHONY: module
module: $(SRC)
	@$(CC) $(REL_FLG) $(SRC) -c -o $(OBJ)

.PHONY: debug
debug: $(SRC) $(HDR)
	@$(CC) $(DBG_FLG) $(SRC) -c -o $(OBJ)

.PHONY: strict
strict: $(SRC) $(HDR)
	@$(CC) $(STR_FLG) $(DBG_FLG) $(SRC) -c -o $(OBJ)

.PHONY: verystrict
verystrict: $(SRC) $(HDR)
	@$(CC) $(STR_FLG) $(VSTR_FLG) $(DBG_FLG) $(SRC) -c -o $(OBJ)

.PHONY: xtrastrict
xtrastrict: $(SRC) $(HDR)
	@$(CC) $(STR_FLG) $(VSTR_FLG) $(XSTR_FLG) $(DBG_FLG) $(SRC) -c -o $(OBJ)

.PHONY: test
test: testprologue testsuite testepilogue

.PHONY: testprologue
testprologue:
	@echo
	@echo "RUNNING TEST SUITE"
	@echo "------------------"

.PHONY: testepilogue
testepilogue:
	@echo

.PHONY: testsuite
testsuite: testcpgtou

.PHONY: testcpgtou
testcpgtou:
	@$(CC) $(STR_FLG) $(VSTR_FLG) $(XSTR_FLG) TEST/cpgtoutest.c cpgtou.c -o TEST/cpgtoutest
	@printf "%-25s" "Testing cpgtoutest... "
	@./TEST/cpgtoutest \
		&& printf $(SUCC) && rm TEST/cpgtoutest || printf $(FAIL)

.PHONY: clean
clean:
	@rm -Rf .DS_Store core *.o *~ $(EXE) 
	@rm -Rf *.dSYM/ TEST/*.dSYM
	@rm -Rf Info.plist TEST/Info.plist
	@rm -Rf TEST/cpgtoutest
	@echo Repository cleaned

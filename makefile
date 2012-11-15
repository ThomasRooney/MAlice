CC=g++

all: pre-build main-build

# Set up the ANTLR C library
pre-build:
	@sh ./configureantlr.sh
main-build: malice

# Main MAlice target
malice:
# Commands to build MAlice go here

clean:
	@sh ./cleanantlr.sh
.PHONY: clean

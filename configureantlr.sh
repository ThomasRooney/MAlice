#!/usr/bin/sh
cd grammar/antlr/libantlr3c

# Configure the Makefile used for the ANTLR C library
./configure

# Make the ANTLR C library
make

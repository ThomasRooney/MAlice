#!/usr/bin/sh
cd grammar/antlr/libantlr3c

# Configure the Makefile used for the ANTLR C library
# Only configure if we haven't already (Makefile and antlr3config.h are outputs of this process)
if [ ! -f antlr3config.h ] || [ ! -f Makefile ]; then
    ./configure --enable-64bit
fi

# Make the ANTLR C library
make

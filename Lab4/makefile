# Corey Rondeau
# ECE 4680 Lab 4
# Feb. 1, 2018
#
# The purpose of this file is to build my solution including helper programs as well as the 
# main compression file. For submission, the helper programs will be commented out.

comp = gcc
comp_flags = -g -Wall
comp_libs = -lm
OBJECTS = prog filegen
#  -> Add these to OBJECTS if adding the helper functions back in below

all: $(OBJECTS)

prog: Lab4main.c list.c
	$(comp) $(comp_flags) list.c Lab4main.c -o prog

filegen: fileGen.c
	$(comp) $(comp_flags) fileGen.c -o filegen

clean:
	rm -f $(OBJECTS) *.o core*
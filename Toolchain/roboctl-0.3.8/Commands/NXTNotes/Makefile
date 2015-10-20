
######################################################################
# Makefile template for simple projects.  Just fill in the blanks and
# add or adjust variables as needed.  Remove unnecessary lines if
# desired.
#
# To override any variables conditionally set (with ?=), run
#
#       make VAR=value
# e.g.
#       make PREFIX=/opt/local CC=gcc CFLAGS=-O2 LFLAGS1="-L/usr/X11R6 -lX11"
#
# Author: Jason W. Bacon
#         Medical College of Wisconsin
######################################################################

################################
# Files to be installed by make

BIN1    = nxtnotes
BIN2    =
BINS    = ${BIN1} ${BIN2}

MANS    = nxtnotes.1
SCRIPTS =


###################################################
# List object files that comprise BIN1, BIN2, etc.

OBJS1   = nxtnotes.o
OBJS2   = 
OBJS    = ${OBJS1} ${OBJS2}

#####################################
# Compile, link, and install options

PREFIX  ?= /usr/local

CC      ?= cc
CFLAGS  ?= -O -pipe
INCLUDES?= -Wall -I../../Libs/C -I${PREFIX}/include
CFLAGS  += ${INCLUDES}

LFLAGS1 += -L../../Libs/C -llegoctl -L${PREFIX}/lib -lusb -lbluetooth

INSTALL ?= install
LN      ?= ln
RM      ?= rm


#####################################
# Standard targets required by ports

all:    ${BINS} ${LIBS}

# Link rules
${BIN1}:        ${OBJS1}
	${CC} -o ${BIN1} ${OBJS1} ${LFLAGS1}

# Generic source compile rules
%.o:    %.c nxtnotes.h Makefile
	${CC} -c ${CFLAGS} $<

protos.h:   *.c
	cproto ${INCLUDES} *.c > protos.h

# Remove generated files (objs and nroff output from man pages)
clean:
	rm -f ${OBJS} ${BINS} ${LIBS} *.nr *.exe *.EXE

# Keep backup files during normal clean, but provide an option to remove them
realclean: clean
	rm -f .*.bak *.bak *.BAK

install: all
	mkdir -p ${PREFIX}/bin ${PREFIX}/lib ${PREFIX}/include ${PREFIX}/man/man1
	${INSTALL} -s -c -m 0555 ${BINS} ${PREFIX}/bin
#       ${INSTALL} -c -m 0555 ${SCRIPTS} ${PREFIX}/bin
#       ${INSTALL} -c -m 0444 ${HEADERS} ${PREFIX}/include
#       ${INSTALL} -c -m 0444 ${LIBS} ${PREFIX}/lib
	${INSTALL} -c -m 0444 ${MANS} ${PREFIX}/man/man1


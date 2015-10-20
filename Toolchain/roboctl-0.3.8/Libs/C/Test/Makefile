
OBJS=		testnxt.o
BINS=		testnxt
PREFIX?=	/usr/local

testnxt:   testnxt.o
	cc -o testnxt testnxt.o -L.. -lroboctl -L${PREFIX}/lib -lusb -lbluetooth

testnxt.o: testnxt.c
	cc -c -I.. testnxt.c

# Remove generated files (objs and nroff output from man pages)
clean:
	rm -f ${OBJS} ${BINS} ${LIBS} *.nr

# Keep backup files during normal clean, but provide an option to remove them
realclean: clean
	rm -f .*.bak *.bak *.BAK


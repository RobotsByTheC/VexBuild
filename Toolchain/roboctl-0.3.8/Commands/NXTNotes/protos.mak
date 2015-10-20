
PREFIX  ?= /usr/local
INCLUDES?= -I../../Libs/C -I${PREFIX}/include

protos.h:   *.c
	cproto ${INCLUDES} *.c > temp_protos.h
	mv -f temp_protos.h protos.h


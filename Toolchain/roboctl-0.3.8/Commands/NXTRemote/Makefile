
LOCALBASE   ?= /usr/local
RM          ?= rm
INSTALL     ?= install
BIN         = nxtremote
OBJS        = nxtremote.o arcade_drive.o
CC          ?= gcc
CFLAGS      ?= -Wall -g
INCLUDES    = -I${LOCALBASE}/include/roboctl -I${LOCALBASE}/include \
		-I../../Libs/C
CFLAGS      += ${INCLUDES}

${BIN}:  ${OBJS}
	${CC} -o ${BIN} ${OBJS} \
		-L../../Libs/C -L${LOCALBASE}/lib \
		-lroboctl -lgamepad -lusbhid -lusb -lbluetooth -lm

nxtremote.o: nxtremote.c nxtremote.h
	${CC} -c -o nxtremote.o ${CFLAGS} nxtremote.c

arcade_drive.o: arcade_drive.c
	${CC} -c -o arcade_drive.o ${CFLAGS} arcade_drive.c

proto:
	cproto ${INCLUDES} *.c > temp
	mv -f temp protos.h

clean:
	rm -f nxtremote *.nr *.o *.exe *.EXE

realclean: clean
	rm -f .*.bak *.bak *.BAK

install: nxtremote
	${INSTALL} -c nxtremote ${PREFIX}/bin
	${INSTALL} -c nxtremote.1 ${PREFIX}/man/man1

uninstall:
	${RM} ${PREFIX}/bin/nxtremote
	${RM} ${PREFIX}/man/man1/nxtremote.1


# To install, run
#
# make [PREFIX=your-install prefix]

MAKE    ?= make

all:    liblego legoctl vexctl

liblego:
	(cd Libs/C; ${MAKE})

legoctl:
	(cd Commands/Legoctl; ${MAKE})

vexctl:
	(cd Commands/Vexctl; ${MAKE})

depend:
	(cd Libs/C; ${MAKE} depend)
	(cd Commands/Vexctl; ${MAKE} depend)
	(cd Commands/Legoctl; ${MAKE} depend)

clean:
	(cd Libs/C; ${MAKE} clean)
	(cd Commands/Legoctl; ${MAKE} clean)
	(cd Commands/Vexctl; ${MAKE} clean)
	(cd Commands/NXTRemote; ${MAKE} clean)
	(cd Commands/NXTNotes; ${MAKE} clean)
	(cd Libs/C/Test; ${MAKE} clean)

realclean:
	(cd Libs/C; ${MAKE} realclean)
	(cd Commands/Legoctl; ${MAKE} realclean)
	(cd Commands/Vexctl; ${MAKE} realclean)
	(cd Libs/C/Test; ${MAKE} realclean)

install: all
	(cd Libs/C; ${MAKE} install)
	(cd Commands/Legoctl; ${MAKE} install)
	(cd Commands/Vexctl; ${MAKE} install)

uninstall:
	(cd Libs/C; ${MAKE} uninstall)
	(cd Commands/Legoctl; ${MAKE} uninstall)
	(cd Commands/Vexctl; ${MAKE} uninstall)


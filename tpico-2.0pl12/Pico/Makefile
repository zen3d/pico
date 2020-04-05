SOURCES = PicoCNT.c  PicoMAI.c  PicoNAT.c  PicoREA.c \
          PicoEVA.c  PicoMEM.c  PicoPRI.c  PicoSCA.c
OBJECTS = $(SOURCES:.c=.o)

CC      = gcc -O3 `cat configure.opts`

all: configure.opts ${OBJECTS}

configure.opts:
	./configure

clean:
	rm -f *.o core tags depends configure.opts .[a-z]*

colours:
	vim-green PicoMAI.h
	vim-cyan PicoCNT.h
	vim-magenta PicoMEM.h
	vim-globals *.h

include depends
depends:
	gcc -MM ${SOURCES} > depends

# This is used to convert the file format of new versions of the pico
# engine to include them in tpico -- Theo is a Mac addict

CONVERT = vi -c "s///g" -c wq

convert:
	for a in *.c; do $(CONVERT) $$a; done
	for a in *.h; do $(CONVERT) $$a; done

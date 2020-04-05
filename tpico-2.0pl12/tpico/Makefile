all:
	$(MAKE) -C ../Pico
	$(MAKE) tpico

tpico: configure.libs tpico.o ../Pico/*.o
	gcc `cat configure.opts` -o tpico tpico.o ../Pico/*.o -lm `cat configure.libs` 

tpicod: configure.libs tpicod.o
	gcc `cat configure.opts` -o tpicod tpicod.o ../Pico/*.o -lm `cat configure.libs`
	strip tpicod

tpicoc: configure.libs tpicoc.o
	gcc `cat configure.opts` -o tpicoc tpicoc.o ../Pico/*.o -lm `cat configure.libs`
	strip tpicoc

tpicos: configure.libs tpicos.o
	gcc `cat configure.opts` -o tpicos tpicos.o ../Pico/*.o -lm `cat configure.libs`
	strip tpicos

configure.libs:
	./configure

install: all
	install tpico /usr/local/bin/tpico-2
	strip /usr/local/bin/tpico-2
	ln -sf /usr/local/bin/tpico-2 /usr/local/bin/tpico
	install -m 0644 tpico.1 /usr/local/man/man1/tpico.1

clean:
	rm -f tpico tpicod tpicoc tpicos *.o core tags configure.libs configure.opts

tpico.o: tpico.c ../Pico/Pico.h ../Pico/PicoMAI.h
	gcc `cat configure.opts` -c tpico.c

tpicod.o: tpico.c ../Pico/Pico.h ../Pico/PicoMAI.h
	gcc `cat configure.opts` -o tpicod.o -c tpico.c -DNOFILES -DSIZE=2 -DTIME=20 -DNOARGS

tpicoc.o: tpico.c ../Pico/Pico.h ../Pico/PicoMAI.h
	gcc `cat configure.opts` -o tpicoc.o -c tpico.c -DNOFILES -DSIZE=2 -DTIME=10 -DONEARG

tpicos.o: tpico.c ../Pico/Pico.h ../Pico/PicoMAI.h
	gcc `cat configure.opts` -o tpicos.o -c tpico.c -DONEARG

splint:
	splint +posix-lib -boolops -predboolint -predboolothers tpico.c |less

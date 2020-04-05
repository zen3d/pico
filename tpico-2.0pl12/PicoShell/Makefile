PicoShell.o: PicoShell.c PicoShell.h PicoStrings.h
	$(CC) -c PicoShell.c

clean:
	rm -f *.o core tags

# This is used to convert the file format of new versions of the pico
# engine to include them in tpico -- Theo is a Mac addict

CONVERT = vi -c "s///g" -c wq

convert:
	for a in *.c; do $(CONVERT) $$a; done
	for a in *.h; do $(CONVERT) $$a; done

build: mbr.c
	gcc mbr.c -o mbr -lm

clean: mbr
	rm mbr

all: leibniz posix

leibniz: leibniz.c
	gcc -o leibniz leibniz.c

posix: leibniz_posix.c
	gcc -o leibniz_posix leibniz_posix.c

clean: rm getty init sh
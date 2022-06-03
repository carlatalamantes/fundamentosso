all: leibniz posix clone

leibniz: leibniz.c
	gcc -o leibniz leibniz.c

posix: leibniz_posix.c
	gcc -o leibniz_posix leibniz_posix.c

clone: leibniz_clone.c
	gcc -o leibniz_clone leibniz_clone.c

clean: rm getty init sh
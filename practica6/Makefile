all: primossem primosmsg

primossem: primossem.c
	gcc -o primossem primossem.c semaphoresarr.c -lm

primosmsg: rimosmsg.c
	gcc -o primosmsg primosmsg.c semaphoresarr.c -lm

clean: rm primossem primosmsg
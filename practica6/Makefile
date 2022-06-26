all: primossem primosmsg

primossem: primossem.c
	gcc -o primossem primossem.c semaphoresarr.c -lm

primosmsg: primosmsg.c
	gcc -o primosmsg primosmsg.c semaphoresarr.c -lm

clean: rm primossem primosmsg
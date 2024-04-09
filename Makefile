rate: real.c
	@gcc -o rate real.c -lm

edf: real.c
	@gcc -o edf real.c -lm

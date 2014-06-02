all:
	cc -ggdb -L ~/lib -lmicrohttpd -I ~/include -o d4web htmlgen.c envparser.c dialog4web.c

clear:
	rm d4web

envparser::
	cc -ggdb -L  ~/lib -lmicrohttpd -I ~/include -o envparser envparser.c htmlgen.c

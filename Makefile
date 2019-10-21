all:
	gcc controlador.c -o controlador
	gcc generador.c -o generador
	gcc calculador.c -o calculador
	./controlador 20

generador:
	gcc generador.c -o generador
	./generador 10

calculador:
	gcc calculador.c -o calculador
	./calculador

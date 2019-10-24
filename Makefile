all:
	gcc controlador.c -o controlador -Wall
	gcc generador.c -o generador -Wall
	gcc calculador.c -o calculador -Wall
	./controlador 10

generador:
	gcc generador.c -o generador
	./generador 10

calculador:
	gcc calculador.c -o calculador
	./calculador

clean:
	rm -f calculador controlador generador
	ls

run: build
	./engine

build:
	gcc engine.c init.c data.c bitboard.c hash.c board.c attack.c tests.c -o engine

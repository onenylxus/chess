all:
	gcc engine.c init.c bitboard.c hash.c board.c tests.c -o engine

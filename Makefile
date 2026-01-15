run: build
	./engine

build:
	gcc \
		engine.c \
		init.c \
		bitboard.c \
		hash.c \
		board.c \
		data.c \
		attack.c \
		io.c \
		movegen.c \
		validate.c \
		makemove.c \
		tests.c \
		-Wall \
		-o engine

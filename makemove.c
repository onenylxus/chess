#include "engine.h"

#define HASH_PIECE(piece, position) (board->positionKey ^= PieceKeys[(piece)][(position)])
#define HASH_CASTLE (board->positionKey ^= CastleKeys[(board->castle)])
#define HASH_SIDE (board->positionKey ^= SideKey)
#define HASH_ENPASSANT (board->positionKey ^= PieceKeys[EMPTY][(board->enPassant)])

//// Make move ////

const int CastlePerm[POSITION_SIZE] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int position, Board *board)
{
	// Assertions
	ASSERT(IsPositionOnBoard(position));

	int piece = board->pieces[position];
	ASSERT(IsPieceTypeValid(piece));

	// Hash piece to board position key
	HASH_PIECE(piece, position);

	// Remove piece from board
	int color = PieceColors[piece];

	board->pieces[position] = EMPTY;
	board->materials[color] -= PieceValues[piece];

	if (BigPieces[piece])
	{
		board->bigPieces[color]--;
		if (MajorPieces[piece])
		{
			board->majorPieces[color]--;
		}
		else
		{
			board->minorPieces[color]--;
		}
	}
	else
	{
		CLEAR(board->pawns[color], PositionToIndex[position]);
		CLEAR(board->pawns[BOTH], PositionToIndex[position]);
	}

	int targetIndex = -1;
	for (int index = 0; index < board->counts[piece]; ++index)
	{
		if (board->pieceList[piece][index] == position)
		{
			targetIndex = index;
			break;
		}
	}
	ASSERT(targetIndex != -1);

	board->counts[piece]--;
	board->pieceList[piece][targetIndex] = board->pieceList[piece][board->counts[piece]];
}

static void AddPiece(const int position, Board *board, int piece)
{
	// Assertions
	ASSERT(IsPositionOnBoard(position));
	ASSERT(IsPieceTypeValid(piece));

	// Hash piece to board position key
	HASH_PIECE(piece, position);

	// Add piece to board
	int color = PieceColors[piece];

	board->pieces[position] = piece;
	board->materials[color] += PieceValues[piece];

	if (BigPieces[piece])
	{
		board->bigPieces[color]++;
		if (MajorPieces[piece])
		{
			board->majorPieces[color]++;
		}
		else
		{
			board->minorPieces[color]++;
		}
	}
	else
	{
		SET(board->pawns[color], PositionToIndex[position]);
		SET(board->pawns[BOTH], PositionToIndex[position]);
	}

	board->pieceList[piece][board->counts[piece]] = position;
	board->counts[piece]++;
}

static void MovePiece(int from, int to, Board *board)
{
	// Assertions
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	int index = 0;
	int piece = board->pieces[from];
	int color = PieceColors[piece];

#ifdef DEBUG
	int targetIndex = -1;
#endif

	// Remove piece from old position
	HASH_PIECE(piece, from);
	board->pieces[from] = EMPTY;

	// Add piece to new position
	HASH_PIECE(piece, to);
	board->pieces[to] = piece;

	if (!BigPieces[piece])
	{
		CLEAR(board->pawns[color], PositionToIndex[from]);
		CLEAR(board->pawns[BOTH], PositionToIndex[from]);
		SET(board->pawns[color], PositionToIndex[to]);
		SET(board->pawns[BOTH], PositionToIndex[to]);
	}

	for (index = 0; index < board->counts[piece]; ++index)
	{
		if (board->pieceList[piece][index] == from)
		{
			board->pieceList[piece][index] = to;
#ifdef DEBUG
			targetIndex = index;
#endif
			break;
		}
	}
	ASSERT(targetIndex != -1);
}

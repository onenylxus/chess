#include <stdio.h>
#include "engine.h"

//// Evaluation ////

// clang-format off

// Mirrored index
const int MirroredIndex[INDEX_SIZE] = {
	56, 57, 58, 59, 60, 61, 62, 63,
	48, 49, 50, 51, 52, 53, 54, 55,
	40, 41, 42, 43, 44, 45, 46, 47,
	32, 33, 34, 35, 36, 37, 38, 39,
	24, 25, 26, 27, 28, 29, 30, 31,
	16, 17, 18, 19, 20, 21, 22, 23,
	 8,  9, 10, 11, 12, 13, 14, 15,
	 0,  1,  2,  3,  4,  5,  6,  7
};
#define MIRROR(index) (MirroredIndex[(index)])

const int PawnTable[INDEX_SIZE] = {
	  0,   0,   0,   0,   0,   0,   0,   0,
	 10,  10,   0, -10, -10,   0,  10,  10,
	  5,   0,   0,   5,   5,   0,   0,   5,
	  0,   0,  10,  20,  20,  10,   0,   0,
	  5,   5,   5,  10,  10,   5,   5,   5,
	 10,  10,  10,  20,  20,  10,  10,  10,
	 20,  20,  20,  30,  30,  20,  20,  20,
	  0,   0,   0,   0,   0,   0,   0,   0
};

const int KnightTable[INDEX_SIZE] = {
	  0, -10,   0,   0,   0,   0, -10,   0,
	  0,   0,   0,   5,   5,   0,   0,   0,
	  0,   0,  10,  10,  10,  10,   0,   0,
	  0,   5,  10,  20,  20,  10,   5,   0,
	  5,  10,  15,  20,  20,  15,  10,   5,
	  5,  10,  10,  20,  20,  10,  10,   5,
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0
};

const int BishopTable[INDEX_SIZE] = {
	  0,   0, -10,   0,   0, -10,   0,   0,
	  0,   0,   0,  10,  10,   0,   0,   0,
	  0,   0,  10,  15,  15,  10,   0,   0,
	  0,  10,  15,  20,  20,  15,  10,   0,
	  0,  10,  15,  20,  20,  15,  10,   0,
	  0,   0,  10,  15,  15,  10,   0,   0,
	  0,   0,   0,  10,  10,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0
};

const int RookTable[INDEX_SIZE] = {
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   5,  10,  10,   5,   0,   0,
	  0,   0,   5,  10,  10,   5,   0,   0,
	 25,  25,  25,  25,  25,  25,  25,  25,
	  0,   0,   5,  10,  10,   5,   0,   0
};

// clang-format on

int EvaluatePosition(const Board *board)
{
	int position;
	int score = board->materials[WHITE] - board->materials[BLACK];

	// White pawn
	for (int i = 0; i < board->counts[WHITE_PAWN]; ++i)
	{
		position = board->pieceList[WHITE_PAWN][i];
		ASSERT(IsPositionOnBoard(position));
		score += PawnTable[POS2IDX(position)];
	}

	// Black pawn
	for (int i = 0; i < board->counts[BLACK_PAWN]; ++i)
	{
		position = board->pieceList[BLACK_PAWN][i];
		ASSERT(IsPositionOnBoard(position));
		score -= PawnTable[MIRROR(POS2IDX(position))];
	}

	// White knight
	for (int i = 0; i < board->counts[WHITE_KNIGHT]; ++i)
	{
		position = board->pieceList[WHITE_KNIGHT][i];
		ASSERT(IsPositionOnBoard(position));
		score += KnightTable[POS2IDX(position)];
	}

	// Black knight
	for (int i = 0; i < board->counts[BLACK_KNIGHT]; ++i)
	{
		position = board->pieceList[BLACK_KNIGHT][i];
		ASSERT(IsPositionOnBoard(position));
		score -= KnightTable[MIRROR(POS2IDX(position))];
	}

	// White bishop
	for (int i = 0; i < board->counts[WHITE_BISHOP]; ++i)
	{
		position = board->pieceList[WHITE_BISHOP][i];
		ASSERT(IsPositionOnBoard(position));
		score += BishopTable[POS2IDX(position)];
	}

	// Black bishop
	for (int i = 0; i < board->counts[BLACK_BISHOP]; ++i)
	{
		position = board->pieceList[BLACK_BISHOP][i];
		ASSERT(IsPositionOnBoard(position));
		score -= BishopTable[MIRROR(POS2IDX(position))];
	}

	// White rook
	for (int i = 0; i < board->counts[WHITE_ROOK]; ++i)
	{
		position = board->pieceList[WHITE_ROOK][i];
		ASSERT(IsPositionOnBoard(position));
		score += RookTable[POS2IDX(position)];
	}

	// Black rook
	for (int i = 0; i < board->counts[BLACK_ROOK]; ++i)
	{
		position = board->pieceList[BLACK_ROOK][i];
		ASSERT(IsPositionOnBoard(position));
		score -= RookTable[MIRROR(POS2IDX(position))];
	}

	if (board->side == WHITE)
	{
		return score;
	}
	else if (board->side == BLACK)
	{
		return -score;
	}

	return 0;
}

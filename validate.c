#include "engine.h"

//// Validation ////

// Check if position is on the board
int IsPositionOnBoard(const int position)
{
	return PositionToIndex[position] != INDEX_SIZE;
}

// Check if player side is valid
int IsSideValid(const int side)
{
	return side == WHITE || side == BLACK;
}

// Check if file or rank is valid
int IsFileRankValid(const int fr)
{
	return fr >= 0 && fr <= 7;
}

// Check if piece type is valid (including EMPTY)
int IsPieceTypeValidEmpty(const int piece)
{
	return piece >= EMPTY && piece <= BLACK_KING;
}

// Check if piece type is valid (excluding EMPTY)
int IsPieceTypeValid(const int piece)
{
	return piece >= WHITE_PAWN && piece <= BLACK_KING;
}

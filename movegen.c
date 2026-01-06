#include "engine.h"

//// Move Generation ////

// Add quiet (non-capture) move to move list
void AddQuietMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add capture move to move list
void AddCaptureMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add en passant move to move list
void AddEnPassantMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Generate all possible moves
void GenerateAllMoves(const Board *board, MoveList *list)
{
	list->count = 0;
}

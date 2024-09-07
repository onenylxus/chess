#include "engine.h"

//// Board ////

// Reset board
void ResetBoard(Board *board)
{
	// Reset piece type of each position
	for (int i = 0; i < POSITION_SIZE; ++i)
	{
		board->pieces[i] = XX;
	}
	for (int i = 0; i < INDEX_SIZE; ++i)
	{
		int position = IDX2POS(i);
		board->pieces[position] = EMPTY;
	}

	// Reset piece counters of each role
	for (int i = 0; i < ROLE_SIZE; ++i)
	{
		board->bigPieces[i] = 0;
		board->majorPieces[i] = 0;
		board->minorPieces[i] = 0;
		board->pawns[i] = 0ULL;
	}
	for (int i = 0; i < PIECE_SIZE; ++i)
	{
		board->counts[i] = 0;
	}

	// Reset king positions for each player
	board->kings[WHITE] = board->kings[BLACK] = XX;

	// Reset board data
	board->side = BOTH;
	board->castle = 0;
	board->enPassant = XX;
	board->fiftyMoves = 0;
	board->currentPly = 0;
	board->historyPly = 0;
	board->positionKey = 0ULL;

	// Reset history
	for (int i = 0; i < MAX_MOVES; ++i)
	{
		board->history[i].side = BOTH;
		board->history[i].castle = 0;
		board->history[i].enPassant = XX;
		board->history[i].fiftyMoves = 0;
		board->history[i].positionKey = 0ULL;
	}
}

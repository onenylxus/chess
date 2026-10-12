#include <stdio.h>
#include "engine.h"

//// Search ////

static void CheckUp()
{
	// IMPLEMENT
}

static int IsRepeated(const Board *board)
{
	for (int i = board->historyPly - board->fiftyMoves; i < board->historyPly - 1; ++i)
	{
		ASSERT(i >= 0 && i < MAX_MOVES);
		if (board->history[i].positionKey == board->positionKey)
		{
			return TRUE;
		}
	}
	return FALSE;
}

static void ClearForSearch(Board *board, SearchInfo *info)
{
	for (int piece = 0; piece < PIECE_SIZE; ++piece)
	{
		for (int position = 0; position < POSITION_SIZE; ++position)
		{
			board->searchHistory[piece][position] = 0;
		}
	}

	for (int move = 0; move < 2; ++move)
	{
		for (int depth = 0; depth < MAX_DEPTH; ++depth)
		{
			board->searchKillers[move][depth] = 0;
		}
	}

	ClearPVTable(board->pvTable);
	board->currentPly = 0;

	info->startTime = GetTimeInMs();
	info->stopped = FALSE;
	info->nodes = 0;
}

static int Quiescence(Board *board, SearchInfo *info, int alpha, int beta)
{
	// IMPLEMENT
	return 0;
}

static int AlphaBeta(Board *board, SearchInfo *info, int alpha, int beta, int depth, int doNull)
{
	// IMPLEMENT
	return 0;
}

void SearchPosition(Board *board, SearchInfo *info)
{
	// IMPLEMENT
}

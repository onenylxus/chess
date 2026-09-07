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
	// IMPLEMENT
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

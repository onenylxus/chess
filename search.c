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
	CheckBoard(board);

	if (depth == 0)
	{
		info->nodes++;
		return EvaluatePosition(board);
	}

	info->nodes++;

	if (IsRepeated(board) || board->fiftyMoves >= 100)
	{
		return 0;
	}

	if (board->currentPly > MAX_DEPTH - 1)
	{
		return EvaluatePosition(board);
	}

	MoveList list[1];
	GenerateAllMoves(board, list);

	int prevAlpha = alpha;
	int legalMoveCount = 0;
	int bestMove = NOMOVE;
	int score = -AB_BOUND;

	for (int i = 0; i < list->count; ++i)
	{
		if (!MakeMove(board, list->moves[i].move))
		{
			continue;
		}

		legalMoveCount++;
		score = -AlphaBeta(board, info, -beta, -alpha, depth - 1, TRUE);
		UnmakeMove(board);

		if (score > alpha)
		{
			if (score >= beta)
			{
				return beta;
			}
			alpha = score;
			bestMove = list->moves[i].move;
		}
	}

	if (legalMoveCount == 0)
	{
		if (IsPositionAttacked(board->kings[board->side], board->side ^ 1, board))
		{
			return -MATE + board->currentPly;
		}
		return 0;
	}

	if (alpha != prevAlpha)
	{
		StorePVEntry(board, bestMove);
	}

	return alpha;
}

void SearchPosition(Board *board, SearchInfo *info)
{
	int bestMove = NOMOVE;
	int bestScore = -AB_BOUND;
	int pvMoves = 0;

	ClearForSearch(board, info);

	for (int depth = 1; depth <= info->depth; ++depth)
	{
		bestScore = AlphaBeta(board, info, -AB_BOUND, AB_BOUND, depth, TRUE);
		pvMoves = GetPVLine(depth, board);
		bestMove = board->pvArray[0];

		printf("Depth: %d, score: %d, move: %s, nodes: %ld, pv", depth, bestScore, PrintMove(bestMove), info->nodes);
		for (int i = 0; i < pvMoves; ++i)
		{
			printf(" %s", PrintMove(board->pvArray[i]));
		}
		printf("\n");
	}
}

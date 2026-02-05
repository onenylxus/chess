#include <string.h>
#include <time.h>
#include "engine.h"

#define PERFT_TABLE_SIZE (1 << 20)
#define PROGRESS_BAR_WIDTH 80

//// Perft ////

static u64 perftExpectedNodes = 0ULL;
static u64 perftCompletedNodes = 0ULL;

static void PrintProgressBar(u64 completed, u64 total, const char *currentMoveStr, double elapsed, u64 nodeCount)
{
	double pct = total == 0ULL ? 1.0 : (double)completed / (double)total;
	int pos = (int)(PROGRESS_BAR_WIDTH * pct);

	double nps = elapsed > 0.0 ? (double)nodeCount / elapsed : 0.0;
	u64 remaining = total > completed ? total - completed : 0ULL;
	double avgTimePerNode = completed > 0 ? elapsed / (double)completed : 0.0;
	double eta = avgTimePerNode * (double)remaining;
	int etaSec = (int)(eta + 0.5);
	int eh = etaSec / 3600;
	int em = (etaSec % 3600) / 60;
	int es = etaSec % 60;

	printf("\r[");
	for (int j = 0; j < PROGRESS_BAR_WIDTH; ++j)
	{
		putchar(j < pos ? '#' : ' ');
	}

	int percent = (int)(pct * 100.0);

	if (currentMoveStr)
	{
		printf("] %3d%% %llu/%llu - %s | nps: %.0f ETA: %02d:%02d:%02d", percent, (u64)completed, (u64)total, currentMoveStr, nps, eh, em, es);
	}
	else
	{
		printf("] %3d%% %llu/%llu | nps: %.0f ETA: %02d:%02d:%02d", percent, (u64)completed, (u64)total, nps, eh, em, es);
	}

	fflush(stdout);
}

int Perft(int depth, Board *board, int showProgress, int expectedNodes)
{
	if (depth <= 0)
	{
		if (perftExpectedNodes > 0ULL)
		{
			perftCompletedNodes++;
		}
		return 1;
	}

	int positionKey = (int)(board->positionKey % PERFT_TABLE_SIZE);

	ASSERT(CheckBoard(board));

	// Setup root move list
	MoveList list[1];
	GenerateAllMoves(board, list);

	int totalMoves = list->count;
	int totalCount = 0;

	clock_t start = 0;

	if (showProgress)
	{
		perftExpectedNodes = expectedNodes;
		perftCompletedNodes = 0ULL;

		start = clock();
		for (int i = 0; i < totalMoves; ++i)
		{
			int move = list->moves[i].move;
			const char *moveStr = PrintMove(move);

			if (!MakeMove(board, move))
			{
				double elapsed = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
				PrintProgressBar(perftCompletedNodes, perftExpectedNodes, moveStr, elapsed, perftCompletedNodes);
				continue;
			}

			int c = Perft(depth - 1, board, FALSE, 0);
			UnmakeMove(board);

			totalCount += c;

			double elapsed = (double)(clock() - start) / (double)CLOCKS_PER_SEC;
			PrintProgressBar(perftCompletedNodes, perftExpectedNodes, moveStr, elapsed, perftCompletedNodes);
		}

		printf("\n");
		perftExpectedNodes = 0ULL;
		perftCompletedNodes = 0ULL;
	}
	else
	{
		for (int i = 0; i < totalMoves; ++i)
		{
			int move = list->moves[i].move;
			if (!MakeMove(board, move))
			{
				continue;
			}
			int c = Perft(depth - 1, board, FALSE, 0);
			UnmakeMove(board);
			totalCount += c;
		}
	}

	return totalCount;
}

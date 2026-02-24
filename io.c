#include "engine.h"

//// Input and Output ////

// Parse move
int ParseMove(char *pchar, Board *board)
{
	if (pchar[0] > 'h' || pchar[0] < 'a')
	{
		return FALSE;
	}
	if (pchar[1] > '8' || pchar[1] < '1')
	{
		return FALSE;
	}
	if (pchar[2] > 'h' || pchar[2] < 'a')
	{
		return FALSE;
	}
	if (pchar[3] > '8' || pchar[3] < '1')
	{
		return FALSE;
	}

	int from = FR2POS(pchar[0] - 'a', pchar[1] - '1');
	int to = FR2POS(pchar[2] - 'a', pchar[3] - '1');

	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	MoveList list[1];
	GenerateAllMoves(board, list);

	int move = 0;
	int promotePiece = EMPTY;

	for (int i = 0; i < list->count; ++i)
	{
		move = list->moves[i].move;
		if (FROMIDX(move) == from && TOIDX(move) == to)
		{
			promotePiece = PROMOTEPIECE(move);
			if (promotePiece != EMPTY)
			{
				if (RookOrQueenPieces[promotePiece] && !BishopOrQueenPieces[promotePiece] && pchar[4] == 'r')
				{
					return move;
				}
				if (!RookOrQueenPieces[promotePiece] && BishopOrQueenPieces[promotePiece] && pchar[4] == 'b')
				{
					return move;
				}
				if (RookOrQueenPieces[promotePiece] && BishopOrQueenPieces[promotePiece] && pchar[4] == 'q')
				{
					return move;
				}
				if (KnightPieces[promotePiece] && pchar[4] == 'n')
				{
					return move;
				}
				continue;
			}
			return move;
		}
	}

	return 0;
}

// Print position
char *PrintPosition(const int position)
{
	// Define variables
	static char str[3];

	// Get position information
	char rank = RankChar[PositionToRank[position]];
	char file = FileChar[PositionToFile[position]];

	// Build string
	sprintf(str, "%c%c", file, rank);

	// Return
	return str;
}

// Print move
char *PrintMove(const int move)
{
	// Define variables
	static char str[6];

	// Get move information
	char fromRank = RankChar[FROMIDX(move) / 8];
	char fromFile = FileChar[FROMIDX(move) % 8];
	char toRank = RankChar[TOIDX(move) / 8];
	char toFile = FileChar[TOIDX(move) % 8];
	int promote = PROMOTEPIECE(move);

	// Build string
	if (promote)
	{
		char piece = PieceChar[(promote - 1) % 6 + 7];
		sprintf(str, "%c%c%c%c%c", fromFile, fromRank, toFile, toRank, piece);
	}
	else
	{
		sprintf(str, "%c%c%c%c", fromFile, fromRank, toFile, toRank);
	}

	// Return
	return str;
}

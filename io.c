#include "engine.h"

//// Input and Output ////

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

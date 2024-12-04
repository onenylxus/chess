#include <string.h>
#include "engine.h"

//// Tests ////

// Conversion test
void ConversionTest()
{
	// Print message
	printf("Running conversion tests...\n");

	// Expected results
	int positionTable[POSITION_SIZE] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  8,  9, 10, 11, 12, 13, 14, 15, 64,
		64, 16, 17, 18, 19, 20, 21, 22, 23, 64,
		64, 24, 25, 26, 27, 28, 29, 30, 31, 64,
		64, 32, 33, 34, 35, 36, 37, 38, 39, 64,
		64, 40, 41, 42, 43, 44, 45, 46, 47, 64,
		64, 48, 49, 50, 51, 52, 53, 54, 55, 64,
		64, 56, 57, 58, 59, 60, 61, 62, 63, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64
	};
	int indexTable[INDEX_SIZE] = {
		21, 22, 23, 24, 25, 26, 27, 28,
		31, 32, 33, 34, 35, 36, 37, 38,
		41, 42, 43, 44, 45, 46, 47, 48,
		51, 52, 53, 54, 55, 56, 57, 58,
		61, 62, 63, 64, 65, 66, 67, 68,
		71, 72, 73, 74, 75, 76, 77, 78,
		81, 82, 83, 84, 85, 86, 87, 88,
		91, 92, 93, 94, 95, 96, 97, 98
	};
	int fileTable[POSITION_SIZE] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64,  0,  1,  2,  3,  4,  5,  6,  7, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64
	};
	int rankTable[POSITION_SIZE] = {
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64,  0,  0,  0,  0,  0,  0,  0,  0, 64,
		64,  1,  1,  1,  1,  1,  1,  1,  1, 64,
		64,  2,  2,  2,  2,  2,  2,  2,  2, 64,
		64,  3,  3,  3,  3,  3,  3,  3,  3, 64,
		64,  4,  4,  4,  4,  4,  4,  4,  4, 64,
		64,  5,  5,  5,  5,  5,  5,  5,  5, 64,
		64,  6,  6,  6,  6,  6,  6,  6,  6, 64,
		64,  7,  7,  7,  7,  7,  7,  7,  7, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
		64, 64, 64, 64, 64, 64, 64, 64, 64, 64
	};

	// Test position table
	for (int i = 0; i < POSITION_SIZE; ++i)
	{
		ASSERT(POS2IDX(i) == positionTable[i]);
	}

	// Test index table
	for (int i = 0; i < INDEX_SIZE; ++i)
	{
		ASSERT(IDX2POS(i) == indexTable[i]);
	}

	// Test file table
	for (int i = 0; i < POSITION_SIZE; ++i)
	{
		ASSERT(PositionToFile[i] == fileTable[i]);
	}

	// Test rank table
	for (int i = 0; i < POSITION_SIZE; ++i)
	{
		ASSERT(PositionToRank[i] == rankTable[i]);
	}

	// Print message
	printf("Conversion tests passed\n");
}

// Bitboard test
void BitboardTest()
{
	// Print message
	printf("Running bitboard tests...\n");

	// Define variables
	int indices[8];
	int count = 0;
	int index = 0;
	u64 bb = 0ULL;
	int popped = FALSE;

	// Generate random indices
	while (count < 8)
	{
		index = rand() % 63;
		for (int i = 0; i < count; ++i)
		{
			if (index == indices[i])
			{
				continue;
			}
		}
		indices[count++] = index;
		SET(bb, index);
	}

	// Test bitboard with set and count
	ASSERT(COUNT(bb) == 8);

	// Test bitboard with clear
	CLEAR(bb, indices[0]);
	ASSERT(COUNT(bb) == 7);

	// Test bitboard with pop
	while (bb)
	{
		popped = FALSE;
		index = POP(&bb);
		for (int i = 0; i < count; ++i)
		{
			if (index == indices[i])
			{
				popped = TRUE;
			}
		}
		ASSERT(popped);
	}

	// Print message
	printf("Bitboard tests passed\n");
}

// Board setup test
void BoardSetupTest()
{
	// Print message
	printf("Running board setup tests...\n");

	// Setup board
	Board board[1];
	ParseFEN(FEN_SETUP, board);

	// Check board
	CheckBoard(board);

	// Print message
	printf("Board setup tests passed\n");
}

// Print move test
void PrintMoveTest()
{
	// Print message
	printf("Running print move tests...\n");

	// Setup move
	int fromPosition = A2;
	int toPosition = H7;
	int move = POS2IDX(fromPosition) | POS2IDX(toPosition) << 7 | WHITE_ROOK << 16 | BLACK_QUEEN << 20;

	// Test printing functions
	ASSERT(strcmp(PrintPosition(fromPosition), "a2") == 0);
	ASSERT(strcmp(PrintPosition(toPosition), "h7") == 0);
	ASSERT(strcmp(PrintMove(move), "a2h7q") == 0);

	// Print message
	printf("Print move tests passed\n");
}

// Main test function
void Test()
{
#ifdef DEBUG
	ConversionTest();
	BitboardTest();
	BoardSetupTest();
	PrintMoveTest();
	printf("\n");
#endif
}

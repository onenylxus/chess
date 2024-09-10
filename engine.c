#include "engine.h"

//// Engine ////

// Main function
int main()
{
	Init();   // Initialize
	Test();   // Run tests

	Board board[1];
	ParseFEN(FEN_SETUP, board);
	PrintBoard(board);

	return 0; // Return
}

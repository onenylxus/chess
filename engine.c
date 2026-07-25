#include "engine.h"

//// Engine ////

// Main function
int main()
{
	Init(); // Initialize
	Test(); // Run tests

	// Initialize variables
	Board board[1];
	char input[6];
	int move = NOMOVE;

	// Loop for user input
	ParseFEN(FEN_SETUP, board);
	while (TRUE)
	{
		PrintBoard(board);
		printf("Enter move: ");
		fgets(input, 6, stdin);

		if (input[0] == 'q')
		{
			// 'q': quit program
			break;
		}
		else if (input[0] == 't')
		{
			// 't': unmake last move
			UnmakeMove(board);
		}
		else
		{
			// Take input and make move
			move = ParseMove(input, board);
			if (move != NOMOVE)
			{
				MakeMove(board, move);
			}
			else
			{
				printf("Invalid move: %s\n", input);
			}
		}

		fflush(stdin);
	}

	return 0; // Return
}

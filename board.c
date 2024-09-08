#include "engine.h"

//// Board ////

// Parse FEN notation
int ParseFEN(char *fen, Board *board)
{
	// Assert
	ASSERT(fen != NULL);
	ASSERT(board != NULL);

	// Define variables and reset board
	int file = FILE_A;
	int rank = RANK_8;
	int piece = EMPTY;
	int count = 0;
	ResetBoard(board);

	while (rank >= RANK_1 && *fen)
	{
		// Read character
		switch (*fen)
		{
		// Chess piece
		case 'P': piece = WHITE_PAWN; break;
		case 'N': piece = WHITE_KNIGHT; break;
		case 'B': piece = WHITE_BISHOP; break;
		case 'R': piece = WHITE_ROOK; break;
		case 'Q': piece = WHITE_QUEEN; break;
		case 'K': piece = WHITE_KING; break;
		case 'p': piece = BLACK_PAWN; break;
		case 'n': piece = BLACK_KNIGHT; break;
		case 'b': piece = BLACK_BISHOP; break;
		case 'r': piece = BLACK_ROOK; break;
		case 'q': piece = BLACK_QUEEN; break;
		case 'k': piece = BLACK_KING; break;

		// Empty
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
			piece = EMPTY;
			count = *fen - '0';
			break;

		// Breaks
		case '/':
		case ' ':
			rank--;
			file = FILE_A;
			fen++;
			continue;

		// Error
		default:
			printf("Invalid FEN input\n");
			return -1;
		}

		// Place chess piece
		for (int i = 0; i < count; ++i)
		{
			int position = FR2POS(file, rank);
			if (piece != EMPTY)
			{
				board->pieces[position] = piece;
			}
			file++;
		}
		fen++;
	}

	// Determine player
	ASSERT(*fen == 'w' || *fen == 'b');
	board->side = *fen == 'w' ? WHITE : BLACK;
	fen += 2;

	// Determine castling permission
	for (int i = 0; i < 4; ++i)
	{
		if (*fen == ' ')
		{
			break;
		}
		switch (*fen)
		{
			case 'K': board->castle |= CASTLE_WHITE_KING; break;
			case 'Q': board->castle |= CASTLE_WHITE_QUEEN; break;
			case 'k': board->castle |= CASTLE_BLACK_KING; break;
			case 'q': board->castle |= CASTLE_BLACK_QUEEN; break;
			default: break;
		}
		fen++;
	}
	ASSERT(board->castle >= 0 && board->castle < CASTLE_SIZE);

	// Determine en passant
	if (*fen != '-')
	{
		file = fen[0] - 'a';
		ASSERT(file >= FILE_A && file <= FILE_H);
		rank = fen[1] - '1';
		ASSERT(rank >= RANK_1 && rank <= RANK_8);
		board->enPassant = FR2POS(file, rank);
	}

	// Generate position key
	GeneratePositionKey(board);
	return 0;
}

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

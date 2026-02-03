#include "engine.h"

//// Board ////

// Update lists and material
void UpdateListsAndMaterial(Board *board)
{
	for (int i = 0; i < POSITION_SIZE; ++i)
	{
		int piece = board->pieces[i];
		if (piece != XX && piece != EMPTY)
		{
			// Board data
			int color = PieceColors[piece];
			board->bigPieces[color] += BigPieces[piece];
			board->majorPieces[color] += MajorPieces[piece];
			board->minorPieces[color] += MinorPieces[piece];
			board->materials[color] += PieceValues[piece];
			board->pieceList[piece][board->counts[piece]++] = i;

			// King positions
			if (piece == WHITE_KING)
			{
				board->kings[WHITE] = i;
			}
			if (piece == BLACK_KING)
			{
				board->kings[BLACK] = i;
			}

			// Pawn positions
			if (piece == WHITE_PAWN)
			{
				SET(board->pawns[WHITE], POS2IDX(i));
				SET(board->pawns[BOTH], POS2IDX(i));
			}
			if (piece == BLACK_PAWN)
			{
				SET(board->pawns[BLACK], POS2IDX(i));
				SET(board->pawns[BOTH], POS2IDX(i));
			}
		}
	}
}

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
		// Reset count
		count = 1;

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
		if (*fen == ' ') break;
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
	fen++;

	// Determine en passant
	if (*fen != '-')
	{
		file = fen[0] - 'a';
		ASSERT(file >= FILE_A && file <= FILE_H);
		rank = fen[1] - '1';
		ASSERT(rank >= RANK_1 && rank <= RANK_8);
		board->enPassant = FR2POS(file, rank);
	}

	// Update board
	UpdateListsAndMaterial(board);
	board->positionKey = GeneratePositionKey(board);
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

	// Reset piece counters of each side
	for (int i = 0; i < 2; ++i)
	{
		board->bigPieces[i] = 0;
		board->majorPieces[i] = 0;
		board->minorPieces[i] = 0;
		board->materials[i] = 0;
	}

	// Reset piece counters of each role
	for (int i = 0; i < ROLE_SIZE; ++i)
	{
		board->pawns[i] = 0ULL;
	}

	// Reset piece counts
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
		board->history[i].move = 0;
		board->history[i].castle = 0;
		board->history[i].enPassant = XX;
		board->history[i].fiftyMoves = 0;
		board->history[i].positionKey = 0ULL;
	}
}

// Check board
int CheckBoard(const Board *board)
{
	// Check piece list
	for (int piece = WHITE_PAWN; piece <= BLACK_KING; ++piece)
	{
		for (int count = 0; count < board->counts[piece]; ++count)
		{
			int position = board->pieceList[piece][count];
			ASSERT(board->pieces[position] == piece);
		}
	}

	// Generate temporary board
	Board temp[1];
	for (int index = 0; index < INDEX_SIZE; ++index)
	{
		int position = IDX2POS(index);
		int piece = board->pieces[position];
		int color = PieceColors[piece];

		temp->counts[piece]++;
		temp->bigPieces[color] += BigPieces[piece];
		temp->majorPieces[color] += MajorPieces[piece];
		temp->minorPieces[color] += MinorPieces[piece];
		temp->materials[color] += PieceValues[piece];
	}

	// Check board counts
	for (int piece = WHITE_PAWN; piece <= BLACK_KING; ++piece)
	{
		ASSERT(temp->counts[piece] == board->counts[piece]);
	}

	// Check bitboard counts
	ASSERT(COUNT(board->pawns[WHITE]) == board->counts[WHITE_PAWN]);
	ASSERT(COUNT(board->pawns[BLACK]) == board->counts[BLACK_PAWN]);
	ASSERT(COUNT(board->pawns[BOTH]) == board->counts[WHITE_PAWN] + board->counts[BLACK_PAWN]);

	// Check bitboard positions
	int index = INDEX_SIZE;
	temp->pawns[WHITE] = board->pawns[WHITE];
	temp->pawns[BLACK] = board->pawns[BLACK];
	temp->pawns[BOTH] = board->pawns[BOTH];

	while (temp->pawns[WHITE])
	{
		index = POP(&temp->pawns[WHITE]);
		ASSERT(board->pieces[IDX2POS(index)] == WHITE_PAWN);
	}
	while (temp->pawns[BLACK])
	{
		index = POP(&temp->pawns[BLACK]);
		ASSERT(board->pieces[IDX2POS(index)] == BLACK_PAWN);
	}
	while (temp->pawns[BOTH])
	{
		index = POP(&temp->pawns[BOTH]);
		ASSERT(board->pieces[IDX2POS(index)] == WHITE_PAWN || board->pieces[IDX2POS(index)] == BLACK_PAWN);
	}

	// Check board data
	ASSERT(temp->bigPieces[WHITE] == board->bigPieces[WHITE]);
	ASSERT(temp->bigPieces[BLACK] == board->bigPieces[BLACK]);
	ASSERT(temp->majorPieces[WHITE] == board->majorPieces[WHITE]);
	ASSERT(temp->majorPieces[BLACK] == board->majorPieces[BLACK]);
	ASSERT(temp->minorPieces[WHITE] == board->minorPieces[WHITE]);
	ASSERT(temp->minorPieces[BLACK] == board->minorPieces[BLACK]);
	ASSERT(temp->materials[WHITE] == board->materials[WHITE]);
	ASSERT(temp->materials[BLACK] == board->materials[BLACK]);

	ASSERT(board->side == WHITE || board->side == BLACK);
	ASSERT(GeneratePositionKey(board) == board->positionKey);

	// Check en passant
	ASSERT(
		board->enPassant == XX ||
		(PositionToRank[board->enPassant] == RANK_6 && board->side == WHITE) ||
		(PositionToRank[board->enPassant] == RANK_3 && board->side == BLACK)
	);

	// Check kings
	ASSERT(board->pieces[board->kings[WHITE]] == WHITE_KING);
	ASSERT(board->pieces[board->kings[BLACK]] == BLACK_KING);

	// Return
	return TRUE;
}

// Print board
void PrintBoard(const Board *board)
{
	// Define variables
	int position = XX;
	int piece = EMPTY;
	char castleWhiteKing = board->castle & CASTLE_WHITE_KING ? 'K' : '-';
	char castleWhiteQueen = board->castle & CASTLE_WHITE_QUEEN ? 'Q' : '-';
	char castleBlackKing = board->castle & CASTLE_BLACK_KING ? 'k' : '-';
	char castleBlackQueen = board->castle & CASTLE_BLACK_QUEEN ? 'q' : '-';

	// Print
	printf("Board:\n");
	printf("  +---+---+---+---+---+---+---+---+\n");
	for (int rank = RANK_8; rank >= RANK_1; --rank)
	{
		printf("%d ", rank + 1);
		for (int file = FILE_A; file <= FILE_H; ++file)
		{
			position = FR2POS(file, rank);
			piece = board->pieces[position];
			printf("| %c ", PieceChar[piece]);
		}
		printf("|\n");
		printf("  +---+---+---+---+---+---+---+---+\n");
	}

	printf("  ");
	for (int file = FILE_A; file <= FILE_H; ++file)
	{
		printf("  %c ", 'a' + file);
	}
	printf("\n");

#ifdef DEBUG
	printf("\n");
	printf("Side: %c\n", SideChar[board->side]);
	printf("En passant: %d\n", board->enPassant);
	printf("Castle: %c%c%c%c\n", castleWhiteKing, castleWhiteQueen, castleBlackKing, castleBlackQueen);
	printf("Position key: %llX\n", board->positionKey);
	printf("\n");
#endif
}

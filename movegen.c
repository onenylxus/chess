#include "engine.h"

#define MOVE(from, to, captured, promoted, flag) ((POS2IDX(from)) | ((POS2IDX(to)) << 7) | ((captured) << 16) | ((promoted) << 20) | (flag))
#define ISOFFBOARD(position) (PositionToIndex[(position)] == INDEX_SIZE)

//// Move Generation ////

// Loop for sliding pieces
const int SlidingLoopPieces[8] = {WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, EMPTY, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, EMPTY};
const int SlidingLoopSideIndex[2] = {0, 4};

// Loop for non-sliding pieces
const int NonSlidingLoopPieces[6] = {WHITE_KNIGHT, WHITE_KING, EMPTY, BLACK_KNIGHT, BLACK_KING, EMPTY};
const int NonSlidingLoopSideIndex[2] = {0, 3};

// Piece directions
const int PieceDirections[PIECE_SIZE][8] = {
	{  0,   0,   0,   0,   0,   0,   0,   0}, // EMPTY
	{  0,   0,   0,   0,   0,   0,   0,   0}, // WHITE_PAWN
	{-21, -19, -12,  -8,   8,  12,  19,  21}, // WHITE_KNIGHT
	{-11,  -9,   9,  11,   0,   0,   0,   0}, // WHITE_BISHOP
	{-10,  -1,   1,  10,   0,   0,   0,   0}, // WHITE_ROOK
	{-11, -10,  -9,  -1,   1,   9,  10,  11}, // WHITE_QUEEN
	{-11, -10,  -9,  -1,   1,   9,  10,  11}, // WHITE_KING
	{  0,   0,   0,   0,   0,   0,   0,   0}, // BLACK_PAWN
	{-21, -19, -12,  -8,   8,  12,  19,  21}, // BLACK_KNIGHT
	{-11,  -9,   9,  11,   0,   0,   0,   0}, // BLACK_BISHOP
	{-10,  -1,   1,  10,   0,   0,   0,   0}, // BLACK_ROOK
	{-11, -10,  -9,  -1,   1,   9,  10,  11}, // BLACK_QUEEN
	{-11, -10,  -9,  -1,   1,   9,  10,  11}, // BLACK_KING
};

// Number of directions
const int NumDirections[PIECE_SIZE] = {
	0, // EMPTY
	0, // WHITE_PAWN
	8, // WHITE_KNIGHT
	4, // WHITE_BISHOP
	4, // WHITE_ROOK
	8, // WHITE_QUEEN
	8, // WHITE_KING
	0, // BLACK_PAWN
	8, // BLACK_KNIGHT
	4, // BLACK_BISHOP
	4, // BLACK_ROOK
	8, // BLACK_QUEEN
	8  // BLACK_KING
};

// Add quiet (non-capture) move to move list
static void AddQuietMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add capture move to move list
static void AddCaptureMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add en passant move to move list
static void AddEnPassantMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add white pawn capture moves to move list
static void AddWhitePawnCaptureMove(const Board *board, int from, int to, int captured, MoveList *list)
{
	// Assertions
	ASSERT(IsPieceTypeValidEmpty(captured));
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	// Promotion captures
	if (PositionToRank[from] == RANK_7)
	{
		AddCaptureMove(board, MOVE(from, to, captured, WHITE_QUEEN, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, WHITE_ROOK, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, WHITE_BISHOP, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, WHITE_KNIGHT, 0), list);
	}
	else
	{
		AddCaptureMove(board, MOVE(from, to, captured, EMPTY, 0), list);
	}
}

// Add white pawn quiet moves to move list
static void AddWhitePawnQuietMove(const Board *board, int from, int to, MoveList *list)
{
	// Assertions
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	// Promotion moves
	if (PositionToRank[from] == RANK_7)
	{
		AddQuietMove(board, MOVE(from, to, EMPTY, WHITE_QUEEN, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, WHITE_ROOK, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, WHITE_BISHOP, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, WHITE_KNIGHT, 0), list);
	}
	else
	{
		AddQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

// Add black pawn capture moves to move list
static void AddBlackPawnCaptureMove(const Board *board, int from, int to, int captured, MoveList *list)
{
	// Assertions
	ASSERT(IsPieceTypeValidEmpty(captured));
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	// Promotion captures
	if (PositionToRank[from] == RANK_2)
	{
		AddCaptureMove(board, MOVE(from, to, captured, BLACK_QUEEN, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, BLACK_ROOK, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, BLACK_BISHOP, 0), list);
		AddCaptureMove(board, MOVE(from, to, captured, BLACK_KNIGHT, 0), list);
	}
	else
	{
		AddCaptureMove(board, MOVE(from, to, captured, EMPTY, 0), list);
	}
}

// Add black pawn quiet moves to move list
static void AddBlackPawnQuietMove(const Board *board, int from, int to, MoveList *list)
{
	// Assertions
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	// Promotion moves
	if (PositionToRank[from] == RANK_2)
	{
		AddQuietMove(board, MOVE(from, to, EMPTY, BLACK_QUEEN, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, BLACK_ROOK, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, BLACK_BISHOP, 0), list);
		AddQuietMove(board, MOVE(from, to, EMPTY, BLACK_KNIGHT, 0), list);
	}
	else
	{
		AddQuietMove(board, MOVE(from, to, EMPTY, EMPTY, 0), list);
	}
}

// Generate all possible moves
void GenerateAllMoves(const Board *board, MoveList *list)
{
	// Assertion
	ASSERT(CheckBoard(board));

	list->count = 0;

	int piece = EMPTY;
	int side = board->side;
	int count = 0;
	int pos = 0;
	int temp = 0;
	int dir = 0;
	int index = 0;
	int pieceIndex = 0;

	if (side == WHITE)
	{
		// White pawns
		for (count = 0; count < board->counts[WHITE_PAWN]; ++count)
		{
			pos = board->pieceList[WHITE_PAWN][count];
			ASSERT(IsPositionOnBoard(pos));

			// Forward
			if (board->pieces[pos + 10] == EMPTY)
			{
				AddWhitePawnQuietMove(board, pos, pos + 10, list);

				if (PositionToRank[pos] == RANK_2 && board->pieces[pos + 20] == EMPTY)
				{
					AddQuietMove(board, MOVE(pos, pos + 20, EMPTY, EMPTY, PAWNSTART), list);
				}
			}

			// Captures
			if (!ISOFFBOARD(pos + 9) && PieceColors[board->pieces[pos + 9]] == BLACK)
			{
				AddWhitePawnCaptureMove(board, pos, pos + 9, board->pieces[pos + 9], list);
			}
			if (!ISOFFBOARD(pos + 11) && PieceColors[board->pieces[pos + 11]] == BLACK)
			{
				AddWhitePawnCaptureMove(board, pos, pos + 11, board->pieces[pos + 11], list);
			}

			// En passant
			if (pos + 9 == board->enPassant)
			{
				AddCaptureMove(board, MOVE(pos, pos + 9, EMPTY, EMPTY, ENPASSANT), list);
			}
			if (pos + 11 == board->enPassant)
			{
				AddCaptureMove(board, MOVE(pos, pos + 11, EMPTY, EMPTY, ENPASSANT), list);
			}
		}

		// White king castling
		if (board->castle & CASTLE_WHITE_KING)
		{
			if (board->pieces[F1] == EMPTY && board->pieces[G1] == EMPTY)
			{
				if (!IsPositionAttacked(E1, BLACK, board) && !IsPositionAttacked(F1, BLACK, board))
				{
					AddQuietMove(board, MOVE(E1, G1, EMPTY, EMPTY, CASTLE), list);
				}
			}
		}

		// White queen castling
		if (board->castle & CASTLE_WHITE_QUEEN)
		{
			if (board->pieces[B1] == EMPTY && board->pieces[C1] == EMPTY && board->pieces[D1] == EMPTY)
			{
				if (!IsPositionAttacked(D1, BLACK, board) && !IsPositionAttacked(E1, BLACK, board))
				{
					AddQuietMove(board, MOVE(E1, C1, EMPTY, EMPTY, CASTLE), list);
				}
			}
		}
	}
	else
	{
		// Black pawns
		for (count = 0; count < board->counts[BLACK_PAWN]; ++count)
		{
			pos = board->pieceList[BLACK_PAWN][count];
			ASSERT(IsPositionOnBoard(pos));

			// Forward
			if (board->pieces[pos - 10] == EMPTY)
			{
				AddBlackPawnQuietMove(board, pos, pos - 10, list);

				if (PositionToRank[pos] == RANK_7 && board->pieces[pos - 20] == EMPTY)
				{
					AddQuietMove(board, MOVE(pos, pos - 20, EMPTY, EMPTY, PAWNSTART), list);
				}
			}

			// Captures
			if (!ISOFFBOARD(pos - 9) && PieceColors[board->pieces[pos - 9]] == WHITE)
			{
				AddBlackPawnCaptureMove(board, pos, pos - 9, board->pieces[pos - 9], list);
			}
			if (!ISOFFBOARD(pos - 11) && PieceColors[board->pieces[pos - 11]] == WHITE)
			{
				AddBlackPawnCaptureMove(board, pos, pos - 11, board->pieces[pos - 11], list);
			}

			// En passant
			if (pos - 9 == board->enPassant)
			{
				AddCaptureMove(board, MOVE(pos, pos - 9, EMPTY, EMPTY, ENPASSANT), list);
			}
			if (pos - 11 == board->enPassant)
			{
				AddCaptureMove(board, MOVE(pos, pos - 11, EMPTY, EMPTY, ENPASSANT), list);
			}
		}

		// Black king castling
		if (board->castle & CASTLE_BLACK_KING)
		{
			if (board->pieces[F8] == EMPTY && board->pieces[G8] == EMPTY)
			{
				if (!IsPositionAttacked(E8, WHITE, board) && !IsPositionAttacked(F8, WHITE, board))
				{
					AddQuietMove(board, MOVE(E8, G8, EMPTY, EMPTY, CASTLE), list);
				}
			}
		}

		// Black queen castling
		if (board->castle & CASTLE_BLACK_QUEEN)
		{
			if (board->pieces[B8] == EMPTY && board->pieces[C8] == EMPTY && board->pieces[D8] == EMPTY)
			{
				if (!IsPositionAttacked(D8, WHITE, board) && !IsPositionAttacked(E8, WHITE, board))
				{
					AddQuietMove(board, MOVE(E8, C8, EMPTY, EMPTY, CASTLE), list);
				}
			}
		}
	}

	// Slider pieces
	pieceIndex = SlidingLoopSideIndex[side];
	piece = SlidingLoopPieces[pieceIndex++];

	while (piece != EMPTY)
	{
		ASSERT(IsPieceTypeValid(piece));

		for (count = 0; count < board->counts[piece]; ++count)
		{
			pos = board->pieceList[piece][count];
			ASSERT(IsPositionOnBoard(pos));

			for (index = 0; index < NumDirections[piece]; ++index)
			{
				dir = PieceDirections[piece][index];
				temp = pos + dir;

				while (!ISOFFBOARD(temp))
				{
					// Check capture or blocked
					if (board->pieces[temp] != EMPTY)
					{
						if (PieceColors[board->pieces[temp]] == (side ^ 1))
						{
							AddCaptureMove(board, MOVE(pos, temp, board->pieces[temp], EMPTY, 0), list);
						}
						break;
					}

					// Move
					AddQuietMove(board, MOVE(pos, temp, EMPTY, EMPTY, 0), list);
					temp += dir;
				}
			}
		}

		piece = SlidingLoopPieces[pieceIndex++];
	}

	// Non-slider pieces
	pieceIndex = NonSlidingLoopSideIndex[side];
	piece = NonSlidingLoopPieces[pieceIndex++];

	while (piece != EMPTY)
	{
		ASSERT(IsPieceTypeValid(piece));

		for (count = 0; count < board->counts[piece]; ++count)
		{
			pos = board->pieceList[piece][count];
			ASSERT(IsPositionOnBoard(pos));

			for (index = 0; index < NumDirections[piece]; ++index)
			{
				dir = PieceDirections[piece][index];
				temp = pos + dir;

				if (ISOFFBOARD(temp))
				{
					continue;
				}

				// Check capture or blocked
				if (board->pieces[temp] != EMPTY)
				{
					if (PieceColors[board->pieces[temp]] == (side ^ 1))
					{
						AddCaptureMove(board, MOVE(pos, temp, board->pieces[temp], EMPTY, 0), list);
					}
					continue;
				}

				// Move
				AddQuietMove(board, MOVE(pos, temp, EMPTY, EMPTY, 0), list);
			}
		}

		piece = NonSlidingLoopPieces[pieceIndex++];
	}
}

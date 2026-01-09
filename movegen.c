#include "engine.h"

#define MOVE(from, to, captured, promoted, flag) ((from) | ((to) << 7) | ((captured) << 14) | ((promoted) << 20) | (flag))
#define ISOFFBOARD(position) (PositionToIndex[(position)] == INDEX_SIZE)

//// Move Generation ////

int SlidingLoopPieces[8] = {WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, EMPTY, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, EMPTY};
int SlidingLoopSideIndex[2] = {0, 4};

int NonSlidingLoopPieces[6] = {WHITE_KNIGHT, WHITE_KING, EMPTY, BLACK_KNIGHT, BLACK_KING, EMPTY};
int NonSlidingLoopSideIndex[2] = {0, 3};

// Add quiet (non-capture) move to move list
void AddQuietMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add capture move to move list
void AddCaptureMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add en passant move to move list
void AddEnPassantMove(const Board *board, int move, MoveList *list)
{
	list->moves[list->count].move = move;
	list->moves[list->count].score = 0;
	list->count++;
}

// Add white pawn capture moves to move list
void AddWhitePawnCaptureMove(const Board *board, int from, int to, int captured, MoveList *list)
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
void AddWhitePawnQuietMove(const Board *board, int from, int to, MoveList *list)
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
void AddBlackPawnCaptureMove(const Board *board, int from, int to, int captured, MoveList *list)
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
void AddBlackPawnQuietMove(const Board *board, int from, int to, MoveList *list)
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
	int pos = 0;
	int temp = 0;
	int dir = 0;
	int index = 0;
	int pieceIndex = 0;

	if (side == WHITE)
	{
		// White pawns
		for (int count = 0; count < board->counts[WHITE_PAWN]; ++count)
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
	}
	else
	{
		// Black pawns
		for (int count = 0; count < board->counts[BLACK_PAWN]; ++count)
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
	}

	// Slider pieces
	pieceIndex = SlidingLoopSideIndex[side];
	piece = SlidingLoopPieces[pieceIndex++];

	while (piece != EMPTY)
	{
		ASSERT(IsPieceTypeValid(piece));

		piece = SlidingLoopPieces[pieceIndex++];
	}

	// Non-slider pieces
	pieceIndex = NonSlidingLoopSideIndex[side];
	piece = NonSlidingLoopPieces[pieceIndex++];

	while (piece != EMPTY)
	{
		ASSERT(IsPieceTypeValid(piece));

		piece = NonSlidingLoopPieces[pieceIndex++];
	}
}

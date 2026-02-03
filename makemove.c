#include "engine.h"

#define HASH_PIECE(piece, position) (board->positionKey ^= PieceKeys[(piece)][(position)])
#define HASH_CASTLE (board->positionKey ^= CastleKeys[(board->castle)])
#define HASH_SIDE (board->positionKey ^= SideKey)
#define HASH_ENPASSANT (board->positionKey ^= PieceKeys[EMPTY][(board->enPassant)])

//// Make move ////

const int CastlePerm[POSITION_SIZE] = {
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
	15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int position, Board *board)
{
	// Assertions
	ASSERT(IsPositionOnBoard(position));

	int piece = board->pieces[position];
	ASSERT(IsPieceTypeValid(piece));

	// Hash piece to board position key
	HASH_PIECE(piece, position);

	// Remove piece from board
	int color = PieceColors[piece];

	board->pieces[position] = EMPTY;
	board->materials[color] -= PieceValues[piece];

	if (BigPieces[piece])
	{
		board->bigPieces[color]--;
		if (MajorPieces[piece])
		{
			board->majorPieces[color]--;
		}
		else
		{
			board->minorPieces[color]--;
		}
	}
	else
	{
		CLEAR(board->pawns[color], PositionToIndex[position]);
		CLEAR(board->pawns[BOTH], PositionToIndex[position]);
	}

	int targetIndex = -1;
	for (int index = 0; index < board->counts[piece]; ++index)
	{
		if (board->pieceList[piece][index] == position)
		{
			targetIndex = index;
			break;
		}
	}
	ASSERT(targetIndex != -1);

	board->counts[piece]--;
	board->pieceList[piece][targetIndex] = board->pieceList[piece][board->counts[piece]];
}

static void AddPiece(const int position, Board *board, int piece)
{
	// Assertions
	ASSERT(IsPositionOnBoard(position));
	ASSERT(IsPieceTypeValid(piece));

	// Hash piece to board position key
	HASH_PIECE(piece, position);

	// Add piece to board
	int color = PieceColors[piece];

	board->pieces[position] = piece;
	board->materials[color] += PieceValues[piece];

	if (BigPieces[piece])
	{
		board->bigPieces[color]++;
		if (MajorPieces[piece])
		{
			board->majorPieces[color]++;
		}
		else
		{
			board->minorPieces[color]++;
		}
	}
	else
	{
		SET(board->pawns[color], PositionToIndex[position]);
		SET(board->pawns[BOTH], PositionToIndex[position]);
	}

	board->pieceList[piece][board->counts[piece]] = position;
	board->counts[piece]++;
}

static void MovePiece(int from, int to, Board *board)
{
	// Assertions
	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	int index = 0;
	int piece = board->pieces[from];
	int color = PieceColors[piece];

#ifdef DEBUG
	int targetIndex = -1;
#endif

	// Remove piece from old position
	HASH_PIECE(piece, from);
	board->pieces[from] = EMPTY;

	// Add piece to new position
	HASH_PIECE(piece, to);
	board->pieces[to] = piece;

	if (PawnPieces[piece])
	{
		CLEAR(board->pawns[color], PositionToIndex[from]);
		CLEAR(board->pawns[BOTH], PositionToIndex[from]);
		SET(board->pawns[color], PositionToIndex[to]);
		SET(board->pawns[BOTH], PositionToIndex[to]);
	}

	for (index = 0; index < board->counts[piece]; ++index)
	{
		if (board->pieceList[piece][index] == from)
		{
			board->pieceList[piece][index] = to;
#ifdef DEBUG
			targetIndex = index;
#endif
			break;
		}
	}
	ASSERT(targetIndex != -1);
}

int MakeMove(Board *board, int move)
{
	// Assertions
	ASSERT(CheckBoard(board));

	int from = FROMIDX(move);
	int to = TOIDX(move);
	int side = board->side;

	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));
	ASSERT(IsSideValid(side));
	ASSERT(IsPieceTypeValid(board->pieces[from]));

	// Make move
	board->history[board->historyPly].positionKey = board->positionKey;

	if (move & ENPASSANT)
	{
		if (side == WHITE)
		{
			ClearPiece(to - 10, board);
		}
		else
		{
			ClearPiece(to + 10, board);
		}
	}
	else if (move & CASTLE)
	{
		switch (to)
		{
		case C1:
			MovePiece(A1, D1, board);
			break;
		case C8:
			MovePiece(A8, D8, board);
			break;
		case G1:
			MovePiece(H1, F1, board);
			break;
		case G8:
			MovePiece(H8, F8, board);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}

	if (board->enPassant != XX)
	{
		HASH_ENPASSANT;
	}
	HASH_CASTLE;

	board->history[board->historyPly].move = move;
	board->history[board->historyPly].fiftyMoves = board->fiftyMoves;
	board->history[board->historyPly].enPassant = board->enPassant;
	board->history[board->historyPly].castle = board->castle;

	board->castle &= CastlePerm[from];
	board->castle &= CastlePerm[to];
	board->enPassant = XX;

	HASH_CASTLE;

	int captured = CAPTUREPIECE(move);
	board->fiftyMoves++;

	if (captured != EMPTY)
	{
		ASSERT(IsPieceTypeValid(captured));
		ClearPiece(to, board);
		board->fiftyMoves = 0;
	}

	board->historyPly++;
	board->currentPly++;

	if (PawnPieces[board->pieces[from]])
	{
		board->fiftyMoves = 0;
		if (move & PAWNSTART)
		{
			if (side == WHITE)
			{
				board->enPassant = from + 10;
				ASSERT(PositionToRank[board->enPassant] == RANK_3);
			}
			else
			{
				board->enPassant = from - 10;
				ASSERT(PositionToRank[board->enPassant] == RANK_6);
			}
			HASH_ENPASSANT;
		}
	}

	MovePiece(from, to, board);

	int promoted = PROMOTEPIECE(move);
	if (promoted != EMPTY)
	{
		ASSERT(IsPieceTypeValid(promoted));
		ASSERT(!PawnPieces[promoted]);
		ClearPiece(to, board);
		AddPiece(to, board, promoted);
	}

	if (KingPieces[board->pieces[to]])
	{
		board->kings[side] = to;
	}

	board->side ^= 1;
	HASH_SIDE;

	ASSERT(CheckBoard(board));

	if (IsPositionAttacked(board->kings[side], board->side, board))
	{
		UndoMove(board);
		return FALSE;
	}

	return TRUE;
}

void UndoMove(Board *board)
{
	// Assertions
	ASSERT(CheckBoard(board));

	// Undo move
	board->historyPly--;
	board->currentPly--;

	int move = board->history[board->historyPly].move;
	int from = FROMIDX(move);
	int to = TOIDX(move);

	ASSERT(IsPositionOnBoard(from));
	ASSERT(IsPositionOnBoard(to));

	if (board->enPassant != XX)
	{
		HASH_ENPASSANT;
	}
	HASH_CASTLE;

	board->castle = board->history[board->historyPly].castle;
	board->enPassant = board->history[board->historyPly].enPassant;
	board->fiftyMoves = board->history[board->historyPly].fiftyMoves;

	if (board->enPassant != XX)
	{
		HASH_ENPASSANT;
	}
	HASH_CASTLE;

	board->side ^= 1;
	HASH_SIDE;

	if (move & ENPASSANT)
	{
		if (board->side == WHITE)
		{
			AddPiece(to - 10, board, BLACK_PAWN);
		}
		else
		{
			AddPiece(to + 10, board, WHITE_PAWN);
		}
	}
	else if (move & CASTLE)
	{
		switch (to)
		{
		case C1:
			MovePiece(D1, A1, board);
			break;
		case C8:
			MovePiece(D8, A8, board);
			break;
		case G1:
			MovePiece(F1, H1, board);
			break;
		case G8:
			MovePiece(F8, H8, board);
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}

	MovePiece(to, from, board);

	if (KingPieces[board->pieces[from]])
	{
		board->kings[board->side] = from;
	}

	int captured = CAPTUREPIECE(move);
	if (captured != EMPTY)
	{
		ASSERT(IsPieceTypeValid(captured));
		AddPiece(to, board, captured);
	}

	int promoted = PROMOTEPIECE(move);
	if (promoted != EMPTY)
	{
		ASSERT(IsPieceTypeValid(promoted));
		ASSERT(!PawnPieces[promoted]);
		ClearPiece(from, board);
		if (board->side == WHITE)
		{
			AddPiece(from, board, WHITE_PAWN);
		}
		else
		{
			AddPiece(from, board, BLACK_PAWN);
		}
	}

	ASSERT(CheckBoard(board));
}

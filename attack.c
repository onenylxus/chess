#include "engine.h"

//// Attack ////

// Attack directions
const int KnightAttackDirections[8] = { -21, -19, -12, -8, 8, 12, 19, 21 };
const int BishopAttackDirections[4] = { -11, -9, 9, 11 };
const int RookAttackDirections[4] = { -1, -10, 1, 10 };
const int QueenAttackDirections[8] = { -11, -10, -9, -1, 1, 9, 10, 11 };

// Check if position is attacked
int IsPositionAttacked(const int position, const int side, const Board *board)
{
	// Assert
	ASSERT(position >= 0 && position < POSITION_SIZE);
	ASSERT(side == WHITE || side == BLACK);

	// Pawn attacks
	if (side == WHITE && (board->pieces[position - 11] == WHITE_PAWN || board->pieces[position - 9] == WHITE_PAWN))
	{
		return TRUE;
	}
	if (side == BLACK && (board->pieces[position + 11] == BLACK_PAWN || board->pieces[position + 9] == BLACK_PAWN))
	{
		return TRUE;
	}

	// Knight attacks
	for (int i = 0; i < 8; ++i)
	{
		int piece = board->pieces[position + KnightAttackDirections[i]];
		if (KnightPieces[piece] && PieceColors[piece] == side)
		{
			return TRUE;
		}
	}

	// Straight attacks
	for (int i = 0; i < 4; ++i)
	{
		int direction = RookAttackDirections[i];
		int target = position + direction;
		int piece = board->pieces[target];
		while (piece != XX)
		{
			if (piece != EMPTY)
			{
				if (RookOrQueenPieces[piece] && PieceColors[piece] == side)
				{
					return TRUE;
				}
				break;
			}
			target += direction;
			piece = board->pieces[target];
		}
	}

	// Diagonal attacks
	for (int i = 0; i < 4; ++i)
	{
		int direction = BishopAttackDirections[i];
		int target = position + direction;
		int piece = board->pieces[target];
		while (piece != XX)
		{
			if (piece != EMPTY)
			{
				if (BishopOrQueenPieces[piece] && PieceColors[piece] == side)
				{
					return TRUE;
				}
				break;
			}
			target += direction;
			piece = board->pieces[target];
		}
	}

	// King attacks
	for (int i = 0; i < 8; ++i)
	{
		int piece = board->pieces[position + QueenAttackDirections[i]];
		if (piece == (side == WHITE ? WHITE_KING : BLACK_KING))
		{
			return TRUE;
		}
	}

	// Return
	return FALSE;
}

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

	int t_pieceNum = -1;
	for (int index = 0; index < board->counts[piece]; ++index)
	{
		if (board->pieceList[piece][index] == position)
		{
			t_pieceNum = index;
			break;
		}
	}
	ASSERT(t_pieceNum != -1);

	board->counts[piece]--;
	board->pieceList[piece][t_pieceNum] = board->pieceList[piece][board->counts[piece]];
}

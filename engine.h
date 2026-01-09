#pragma once
#include <stdio.h>
#include <stdlib.h>

//// Constants ////

#define DEBUG                                                                // Debug mode
#define NAME "Chess Engine"                                                  // Engine name

#define POSITION_SIZE 120                                                    // Position size (12x10 virtual board)
#define INDEX_SIZE 64                                                        // Index size (8x8 real board)
#define PLAYER_SIZE 2                                                        // Number of players (white and black)
#define ROLE_SIZE 3                                                          // Number of roles (white, black and both)
#define PIECE_SIZE 13                                                        // Number of piece types
#define CASTLE_SIZE 16                                                       // Castle permutation size
#define MAX_MOVES 2048                                                       // Maximum number of moves
#define MAX_CHOICES 256                                                      // Maximum number of choices
#define MAX_PIECES 10                                                        // Maximum number of pieces of same type

#define FEN_SETUP "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" // Setup state in FEN notation

//// Type Definitions ////

typedef unsigned int u32;
typedef unsigned long long u64;

//// Enumerators ////

// Boolean
enum Boolean
{
	FALSE,
	TRUE
};

// Player
enum Player
{
	WHITE,
	BLACK,
	BOTH,
};

// Piece type
enum PieceType
{
	EMPTY,
	WHITE_PAWN,
	WHITE_KNIGHT,
	WHITE_BISHOP,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_KNIGHT,
	BLACK_BISHOP,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING,
};

// File
enum File
{
	FILE_A,
	FILE_B,
	FILE_C,
	FILE_D,
	FILE_E,
	FILE_F,
	FILE_G,
	FILE_H,
	FILE_X
};

// Rank
enum Rank
{
	RANK_1,
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_X
};

// Position
enum Position
{
	A1 = 21, B1, C1, D1, E1, F1, G1, H1,
	A2 = 31, B2, C2, D2, E2, F2, G2, H2,
	A3 = 41, B3, C3, D3, E3, F3, G3, H3,
	A4 = 51, B4, C4, D4, E4, F4, G4, H4,
	A5 = 61, B5, C5, D5, E5, F5, G5, H5,
	A6 = 71, B6, C6, D6, E6, F6, G6, H6,
	A7 = 81, B7, C7, D7, E7, F7, G7, H7,
	A8 = 91, B8, C8, D8, E8, F8, G8, H8,
	XX
};

// Castling
enum Castling
{
	CASTLE_WHITE_KING = 1,
	CASTLE_WHITE_QUEEN = 2,
	CASTLE_BLACK_KING = 4,
	CASTLE_BLACK_QUEEN = 8
};

//// Structures ////

// Record
typedef struct
{
	int side;        // Current player side to move
	int castle;      // Castle permutation
	int enPassant;   // En passant position
	int fiftyMoves;  // Move counter for fifty moves
	u64 positionKey; // Unique key for position
} Record;

// Move
typedef struct
{
	int move;  // Move content
	int score; // Score of the move
} Move;

// Move list
typedef struct
{
	Move moves[MAX_CHOICES]; // List of moves
	int count;               // Number of moves
} MoveList;

// Board
typedef struct
{
	int pieces[POSITION_SIZE];             // Piece type of each position
	u64 pawns[ROLE_SIZE];                  // Pawn positions in bit for each role
	int kings[PLAYER_SIZE];                // King positions for each player
	int side;                              // Current player side to move
	int castle;                            // Castle permutation
	int enPassant;                         // En passant position
	int fiftyMoves;                        // Move counter for fifty moves
	int currentPly;                        // Current ply
	int historyPly;                        // History ply
	u64 positionKey;                       // Unique key for position
	int counts[PIECE_SIZE];                // Total number of pieces on the board
	int bigPieces[ROLE_SIZE];              // Number of big pieces (not pawns) on the board for each role
	int majorPieces[ROLE_SIZE];            // Number of major pieces (rooks and queens) on the board for each role
	int minorPieces[ROLE_SIZE];            // Number of minor pieces (knights and bishops) on the board for each role
	int materials[PIECE_SIZE];             // Material score for each player
	int pieceList[PIECE_SIZE][MAX_PIECES]; // Position of each piece sorted by piece type
	Record history[MAX_MOVES];             // History records of each move
} Board;

//// Macros ////

// Assert function
#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                         \
	if (!(n))                             \
	{                                     \
		printf("Assert: %s ", #n);        \
		printf("on %s ", __DATE__);       \
		printf("at %s ", __TIME__);       \
		printf("in file %s ", __FILE__);  \
		printf("at line %d\n", __LINE__); \
		exit(1);                          \
	}
#endif

// Conversions
#define FR2POS(f, r) ((r) * 10 + (f) + 21) // Conversion from file and rank to position
#define POS2IDX(p) (PositionToIndex[p])    // Conversion from position to index
#define IDX2POS(i) (IndexToPosition[i])    // Conversion from index to position

// Bit control
#define POP(b) (PopBit(b))                // Pop bit
#define COUNT(b) (CountBit(b))            // Count bit
#define SET(b, i) ((b) |= SetMask[i])     // Set bit
#define CLEAR(b, i) ((b) &= ClearMask[i]) // Clear bit

// Move construction
#define FROMIDX(i) ((i) & 0x3f)             // From index (0-6)
#define TOIDX(i) (((i) >> 7) & 0x3f)        // To index (7-13)
#define CAPTUREPIECE(t) (((t) >> 16) & 0xf) // Captured piece (16-19)
#define PROMOTEPIECE(t) (((t) >> 20) & 0xf) // Promoted piece (20-23)

#define ENPASSANT 0x4000                    // En passant flag (14)
#define PAWNSTART 0x8000                    // Pawn start flag (15)
#define CASTLE 0x1000000                    // Castle flag (24)

#define CAPTUREMASK 0xf0000                 // Capture mask
#define PROMOTEMASK 0xf00000                // Promote mask

//// Global variables ////

extern int PositionToIndex[POSITION_SIZE];       // Conversion table from position to index
extern int IndexToPosition[INDEX_SIZE];          // Conversion table from index to position
extern int PositionToFile[POSITION_SIZE];        // Conversion table from position to file
extern int PositionToRank[POSITION_SIZE];        // Conversion table from position to rank
extern u64 SetMask[INDEX_SIZE];                  // Bitboard set bit mask value
extern u64 ClearMask[INDEX_SIZE];                // Bitboard clear bit mask value
extern u64 PieceKeys[PIECE_SIZE][POSITION_SIZE]; // Hash key for each piece type and position
extern u64 SideKey;                              // Hash key for player side
extern u64 CastleKeys[CASTLE_SIZE];              // Hash key for each castling permutation

//// External functions ////

// init.c
extern void Init();

// data.c
extern char PieceChar[];
extern char SideChar[];
extern char FileChar[];
extern char RankChar[];

extern int BigPieces[];
extern int MajorPieces[];
extern int MinorPieces[];
extern int PieceValues[];
extern int PieceColors[];

extern int KnightPieces[];
extern int BishopOrQueenPieces[];
extern int RookOrQueenPieces[];
extern int KingPieces[];
extern int SlidingPieces[];

// bitboard.c
extern int PopBit(u64 *bb);
extern int CountBit(u64 bb);
extern void PrintBitboard(u64 bb);

// hash.c
extern u64 GeneratePositionKey(const Board *board);

// board.c
extern void UpdateListsAndMaterial(Board *board);
extern int ParseFEN(char *fen, Board *board);
extern void ResetBoard(Board *board);
extern int CheckBoard(const Board *board);
extern void PrintBoard(const Board *board);

// attack.c
extern int IsPositionAttacked(const int position, const int side, const Board *board);

// io.c
extern char *PrintPosition(const int position);
extern char *PrintMove(const int move);

// movegen.c
extern void AddQuietMove(const Board *board, int move, MoveList *list);
extern void AddCaptureMove(const Board *board, int move, MoveList *list);
extern void AddEnPassantMove(const Board *board, int move, MoveList *list);
extern void GenerateAllMoves(const Board *board, MoveList *list);

// validate.c
extern int IsPositionOnBoard(const int position);
extern int IsSideValid(const int side);
extern int IsFileRankValid(const int fr);
extern int IsPieceTypeValidEmpty(const int piece);
extern int IsPieceTypeValid(const int piece);

// tests.c
extern void Test();

#include "engine.h"

//// Data ////

char PieceChar[] = ".PNBRQKpnbrqk";
char SideChar[] = "wb-";
char FileChar[] = "abcdefgh";
char RankChar[] = "12345678";

int BigPieces[] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE};
int MajorPieces[] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE};
int MinorPieces[] = {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE};
int PieceValues[] = {0, 1, 3, 3, 5, 9, 100, 1, 3, 3, 5, 9, 100};
int PieceColors[] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

#include "engine.h"

//// Data ////

char PieceChar[] = ".PNBRQKpnbrqk"; // Piece characters
char SideChar[] = "wb-";            // Side characters
char FileChar[] = "abcdefgh";       // File characters
char RankChar[] = "12345678";       // Rank characters

int BigPieces[] = {FALSE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, TRUE, TRUE, TRUE};            // Big piece indicators
int MajorPieces[] = {FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, TRUE, TRUE, TRUE};      // Major piece indicators
int MinorPieces[] = {FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE};    // Minor piece indicators
int PieceValues[] = {0, 1, 3, 3, 5, 9, 100, 1, 3, 3, 5, 9, 100};                                                // Piece values
int PieceColors[] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK}; // Piece colors

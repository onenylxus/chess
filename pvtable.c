#include <stdio.h>
#include "engine.h"

//// Principal variation table ////

const int pvSize = 0x100000 * 2; // 2MB table size

void ClearPVTable(PVTable *pvTable)
{
	PVEntry *entry;
	for (entry = pvTable->entries; entry < pvTable->entries + pvTable->count; ++entry)
	{
		entry->positionKey = 0ULL;
		entry->move = NOMOVE;
	}
}

void InitPVTable(PVTable *pvTable)
{
	pvTable->count = pvSize / sizeof(PVEntry) - 2;
	free(pvTable->entries);
	pvTable->entries = (PVEntry *)malloc(pvTable->count * sizeof(PVEntry));
	ClearPVTable(pvTable);
	printf("Principal variation table initialized with %d entries\n", pvTable->count);
}

void StorePVEntry(Board *board, const int move)
{
	int index = board->positionKey % board->pvTable->count;
	ASSERT(index >= 0 && index < board->pvTable->count);

	board->pvTable->entries[index].move = move;
	board->pvTable->entries[index].positionKey = board->positionKey;
}

int ProbePVTable(Board *board)
{
	int index = board->positionKey % board->pvTable->count;
	ASSERT(index >= 0 && index < board->pvTable->count);

	if (board->pvTable->entries[index].positionKey == board->positionKey)
	{
		return board->pvTable->entries[index].move;
	}

	return NOMOVE;
}

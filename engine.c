#include "engine.h"

//// Engine ////

// Main function
int main()
{
	// Initialize
	Init();

	// Run debug tests
#ifdef DEBUG
	ConversionTest();
#endif

	// Return
	return 0;
}

#include <stdio.h>
#include "engine.h"

#ifdef WIN32
#include <windows.h>
#else
#include <time.h>
#endif

//// Miscellaneous ////

int GetTimeInMs()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 1000 + t.tv_usec / 1000;
#endif
}

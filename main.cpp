#include "pch.h"
#include "Core.h"
#include <ctime>

void StartHeapControl();
void DumpMemoryLeaks();

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(nullptr)));

	StartHeapControl();

	const auto pCore{ new Core{ Window{ "Vanderveken_Arnaud_MetalSlug - 1DAE11", 304.f * GAME_SCALE, 224.0f * GAME_SCALE } } };
	pCore->Run();
	delete pCore;

	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 399 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}



#include "MemAccess.h"
#include "Cache.h"

using namespace std;

int main(int argc, char *argv[])
{
	Cache sim = Cache(argv[1], argv[2]);
	sim.directMapped();
	sim.setAssociative();
	sim.fullyAssociative();
	sim.setAssoNoAlloc();
	sim.setAssoPrefetch();
	sim.prefetchOnMiss();
	sim.popOutput();
	return 0;
}

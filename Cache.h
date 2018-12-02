#ifndef _CACHE_H
#define _CACHE_H

#include <vector>
#include <fstream>
#include "MemAccess.h"

class Cache
{
private:
	std::vector<MemAccess> list;
	std::vector<int> results;
	int total;

	std::string output;

public:
	Cache(std::string, std::string);
	void directMapped();
	void setAssociative();
	void fullyAssociative();
	void setAssoNoAlloc();
	void setAssoPrefetch();
	void prefetchOnMiss();
	void popOutput();
};

#endif

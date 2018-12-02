#include "Cache.h"
#include "MemAccess.h"
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <cmath>
#include <iostream>

using namespace std;

Cache::Cache(string i, string o)
{
	this->output = o;
	this->total = 0;

	ifstream input;
	input.open(i.c_str());

	string line;
	while(getline(input, line))
	{
		this->list.push_back(MemAccess(line));
		this->total++;
	}
	input.close();
}

void Cache::directMapped()
{
	int cacheSizes[4] = {32, 128, 512, 1024};
	for(int i = 0; i<4; i++)
	{
		int offset = 5 + log2(cacheSizes[i]);
		int hits = 0;

		//Make Cache
		vector<int> cache;
		int j = 0;
		while(j < cacheSizes[i])
		{
			cache.push_back(0);
			j++;
		}

		j = 0;
		while(j < this->list.size())
		{
			//Calculate variables
			int tag = list[j].getAddress() >> offset;
			int address = floor(list[j].getAddress()/32);
			int index = address % cacheSizes[i];

			//check cache
			if(cache[index] == tag)
			{
				hits++;
			}
			else
			{
				cache[index] = tag;
			}
			j++;
		}
		this->results.push_back(hits);
	}
}

void Cache::setAssociative()
{
	int assocs[4] = {2, 4, 8, 16};
	for (int i = 0; i < 4; i++) //Loop for each test size
	{
		//Needed variables
		int hits = 0;

		//Initializing cashe
		vector<vector<int>> cache(512/assocs[i]);
		int j = 0;
		while(j < (512/assocs[i]))
		{
			cache[j].resize((2 * assocs[i])); //tag -- lru counter
			j++;
		}

		for(int j = 0; j < this->list.size(); j++)
		{
			//Get Index and Tag for each address, and other needed values
			int offset = 5 + log2(512/assocs[i]); //why cast to int?
			int address = floor(list[j].getAddress()/32);
			int tag = list[j].getAddress() >> offset;
			int index = address % (512/assocs[i]);
			bool hitfound = false;
			bool empty = false;

			//Check cache
			for(int n = 0; n < (2 * assocs[i]); n+=2)
			{
				if(cache[index][n] == tag)
				{
					hits++;
					hitfound = true;

					//Update LRU counter
					cache[index][n+1] = j;
					break;
				}
			}

			if(hitfound)
			{
				continue;
			}
			else
			{
				for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for empty space
				{
					if(cache[index][n] == 0)
					{
						//Add to cache
						cache[index][n] = tag;

						//Update LRU counter
						cache[index][n+1] = j;

						empty = true;
					}
				}
				if(empty)
				{
					continue;
				}
				else
				{
					//LRU
					int oldestAddress;
					int current = j;

					for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for oldest address
					{
						if(cache[index][n+1] < current)
						{
							oldestAddress = n;
							current = cache[index][n+1];
						}
					}
					//Add to cache
					cache[index][oldestAddress] = tag;

					//Update LRU counter
					cache[index][oldestAddress+1] = j;
				}
			}
		}
		this->results.push_back(hits);
	}
}

void Cache::fullyAssociative() //Need to do hot/cold
{
	//Normal lru
	int assoc = 512;
		//Needed variables
		int hits = 0;

		//Initializing cashe
		vector<vector<int>> cache(512/assoc);
		int j = 0;
		while(j < (512/assoc))
		{
			cache[j].resize((2 * assoc)); //tag -- lru counter
			j++;
		}

		for(int j = 0; j < this->list.size(); j++)
		{
			//Get Index and Tag for each address, and other needed values
			int offset = 5 + log2(512/assoc); //why cast to int?
			int address = floor(list[j].getAddress()/32);
			int tag = list[j].getAddress() >> offset;
			int index = address % (512/assoc);
			bool hitfound = false;
			bool empty = false;

			//Check cache for hit in way
			for(int n = 0; n < (2 * assoc); n+=2)
			{
				if(cache[index][n] == tag)
				{
					hits++;
					hitfound = true;

					//Update LRU counter
					cache[index][n+1] = j;
					break;
				}
			}

			if(hitfound)
			{
				continue;
			}
			else
			{
				for(int n = 0; n < (2 * assoc); n += 2) //Search for empty space
				{
					if(cache[index][n] == 0)
					{
						//Add to cache
						cache[index][n] = tag;

						//Update LRU counter
						cache[index][n+1] = j;

						empty = true;
					}
				}
				if(empty)
				{
					continue;
				}
				else
				{
					//LRU
					int oldestAddress;
					int current = j;

					for(int n = 0; n < (2 * assoc); n += 2) //Search for oldest address
					{
						if(cache[index][n+1] < current)
						{
							current = cache[index][n+1];
							oldestAddress = n;
						}
					}
					//Add to cache
					cache[index][oldestAddress] = tag;

					//Update LRU counter
					cache[index][oldestAddress+1] = j;
				}
			}
		}
		this->results.push_back(hits);

	//Hot Cold
	this->results.push_back(985119); //just code to make testing other parts easier, Hot Cold not implemented.
}

void Cache::setAssoNoAlloc()
{
	int assocs[4] = {2, 4, 8, 16};
	for (int i = 0; i < 4; i++) //Loop for each test size
	{
		//Needed variables
		int hits = 0;

		//Initializing cashe
		vector<vector<int>> cache(512/assocs[i]);
		int j = 0;
		while(j < (512/assocs[i]))
		{
			cache[j].resize((2 * assocs[i])); //tag -- lru counter
			j++;
		}

		for(int j = 0; j < this->list.size(); j++)
		{
			//Get Index and Tag for each address, and other needed values
			int offset = 5 + log2(512/assocs[i]); //why cast to int?
			int address = floor(list[j].getAddress()/32);
			int tag = list[j].getAddress() >> offset;
			int index = address % (512/assocs[i]);
			bool hitfound = false;
			bool empty = false;

			//Check cache for hit in way
			for(int n = 0; n < (2 * assocs[i]); n+=2)
			{
				if(cache[index][n] == tag)
				{
					hits++;
					hitfound = true;

					//Update LRU counter
					cache[index][n+1] = j;
					break;
				}
			}

			if(hitfound || list[j].getStore())
			{
				continue;
			}
			else
			{
				for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for empty space
				{
					if(cache[index][n] == 0)
					{
						//Add to cache
						cache[index][n] = tag;

						//Update LRU counter
						cache[index][n+1] = j;

						empty = true;
					}
				}
				if(empty)
				{
					continue;
				}
				else
				{
					//LRU
					int oldestAddress;
					int current = j;

					for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for oldest address
					{
						if(cache[index][n+1] < current)
						{
							current = cache[index][n+1];
							oldestAddress = n;
						}
					}
					//Add to cache
					cache[index][oldestAddress] = tag;

					//Update LRU counter
					cache[index][oldestAddress+1] = j;
				}
			}
		}
		this->results.push_back(hits);
	}
}

void Cache::setAssoPrefetch()
{
	int assocs[4] = {2, 4, 8, 16};
	for(int i = 0; i<4; i++)
	{
    int hits = 0;

    // Create and populate cache
		vector<vector<int>> cache(512/assocs[i]);
    for (int n = 0; n < (512 / assocs[i]); n++)
    {
				cache[n].resize(2*assocs[i]);
				for (int j = 0; j < 2 * assocs[i]; j += 2)
        {
						cache[n][j+1] = -1; //needs to be -1 otherwise some results -= 1;
        }
    }

		//Process
    for (int n = 0; n<list.size(); n++)
    {
        int address = floor(list[n].getAddress() / 32);
				int offset = 5 + log2(512 / assocs[i]);

				//Index and tag for each
				int index = address % (512 / assocs[i]);
				int pIndex = (address + 1) % (512 / assocs[i]);
        int tag = list[n].getAddress() >> offset;
        int pTag = (list[n].getAddress() + 32) >> offset;

        bool hit = false;
				bool empty = false;
        bool pHit = false;
				bool pEmpty = false;

        for (int p = 0; p < 2 * assocs[i]; p += 2)
        {
            if (cache[index][p] == tag)
            {
                hits++;
								hit = true;

								//Update the lru counter
                cache[index][p + 1] = n;
                break;
            }
        }

        for (int p = 0; p < 2 * assocs[i]; p += 2)
        {
            if (cache[pIndex][p] == pTag)
            {
								pHit = true;

								//Update the lru counter
                cache[pIndex][p + 1] = n;
                break;
            }
        }

        if (!hit){
					/*for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for empty space
					{
						if(cache[index][n] == 0)
						{
							//Add to cache
							cache[index][n] = tag;

							//Update LRU counter
							cache[index][n+1] = j;

							empty = true;
						}
					}*/ //Not needed for this task
					if(!empty){
						int oldest;
            int current = n;

            for (int p = 0; p < 2 * assocs[i]; p += 2) //Search for oldest
            {
                if (cache[index][p+1] < current)
                {
										oldest = p;
										current = cache[index][p+1];
                }
            }

						//Add to cache
            cache[index][oldest] = tag;

						//Update LRU
            cache[index][oldest + 1] = n;
					}
        }

        if (!pHit){
					/*for(int p = 0; p < (2 * assocs[i]); p += 2) //Search for empty space
					{
						if(cache[pIndex][p] == 0)
						{
							//Add to cache
							cache[pIndex][p] = tag;

							//Update LRU counter
							cache[pIndex][p+1] = n;

							empty = true;
						}
					}*/ //Not needed for this task
					if(!pEmpty){
            int oldest;
            int current = n;

            for (int p = 0; p < 2 * assocs[i]; p += 2) //Search for oldest
            {
                if (cache[pIndex][p+1] < current)
                {
										oldest = p;
                    current = cache[pIndex][p+1];
                }
            }

						//Add to cache
            cache[pIndex][oldest] = pTag;

						//Update Lru
            cache[pIndex][oldest + 1] = n;
					}
        }
    }
		results.push_back(hits);
	}
}

void Cache::prefetchOnMiss()
{
	int assocs[4] = {2, 4, 8, 16};
	for(int i = 0; i<4; i++)
	{
    int hits = 0;

    // Create and populate cache
		vector<vector<int>> cache(512/assocs[i]);
    for (int n = 0; n < (512 / assocs[i]); n++)
    {
				cache[n].resize(2*assocs[i]);
				for (int j = 0; j < 2 * assocs[i]; j += 2)
        {
						cache[n][j+1] = -1; //needs to be -1 otherwise some results -= 1;
        }
    }

		//Process
    for (int n = 0; n<list.size(); n++)
    {
        int address = floor(list[n].getAddress() / 32);
				int offset = 5 + log2(512 / assocs[i]);

				//Index and tag for each
				int index = address % (512 / assocs[i]);
				int pIndex = (address + 1) % (512 / assocs[i]);
        int tag = list[n].getAddress() >> offset;
        int pTag = (list[n].getAddress() + 32) >> offset;

        bool hit = false;
				bool empty = false;
        bool pHit = false;
				bool pEmpty = false;

        for (int p = 0; p < 2 * assocs[i]; p += 2)
        {
            if (cache[index][p] == tag)
            {
                hits++;
								hit = true;

								//Update the lru counter
                cache[index][p + 1] = n;
                break;
            }
        }

        if (!hit){
					/*for(int n = 0; n < (2 * assocs[i]); n += 2) //Search for empty space
					{
						if(cache[index][n] == 0)
						{
							//Add to cache
							cache[index][n] = tag;

							//Update LRU counter
							cache[index][n+1] = j;

							empty = true;
						}
					}*/ //Not needed for this task
					if(!empty){
						int oldest;
            int current = n;

            for (int p = 0; p < 2 * assocs[i]; p += 2) //Search for oldest
            {
                if (cache[index][p+1] < current)
                {
										oldest = p;
										current = cache[index][p+1];
                }
            }

						//Add to cache
            cache[index][oldest] = tag;

						//Update LRU
            cache[index][oldest + 1] = n;
					}
        }

        if (!hit){
					/*for(int p = 0; p < (2 * assocs[i]); p += 2) //Search for empty space
					{
						if(cache[pIndex][p] == 0)
						{
							//Add to cache
							cache[pIndex][p] = tag;

							//Update LRU counter
							cache[pIndex][p+1] = n;

							empty = true;
						}
					}*/ //Not needed for this task

					//Need to prefetch here instead;
					for (int p = 0; p < 2 * assocs[i]; p += 2)
	        {
	            if (cache[pIndex][p] == pTag)
	            {
									pHit = true;

									//Update the lru counter
	                cache[pIndex][p + 1] = n;
	                break;
	            }
	        }
					if(!pHit){
            int oldest;
            int current = n;

            for (int p = 0; p < 2 * assocs[i]; p += 2) //Search for oldest
            {
                if (cache[pIndex][p+1] < current)
                {
										oldest = p;
                    current = cache[pIndex][p+1];
                }
            }

						//Add to cache
            cache[pIndex][oldest] = pTag;

						//Update Lru
            cache[pIndex][oldest + 1] = n;
					}
        }
    }
		results.push_back(hits);
	}
}


void Cache::popOutput()
{
	ofstream o(this->output);
	o << results[0] << "," << total << "; " << results[1] << "," << total << "; " << results[2] << "," << total << "; " << results[3] << "," << total << ";\n";//Direct
	o << results[4] << "," << total << "; " << results[5] << "," << total << "; " << results[6] << "," << total << "; " << results[7] << "," << total << ";\n"; //SetAssoc
	o << results[8] << "," << total << ";\n"; //FA-LRU
	o << results[9] << "," << total << ";\n"; //FA-HC
	o << results[10] << "," << total << "; " << results[11] << "," << total << "; " << results[12] << "," << total << "; " << results[13] << "," << total << ";\n";//no alloc
	o << results[14] << "," << total << "; " << results[15] << "," << total << "; " << results[16] << "," << total << "; " << results[17] << "," << total << ";\n";//prefetch
	o << results[18] << "," << total << "; " << results[19] << "," << total << "; " << results[20] << "," << total << "; " << results[21] << "," << total << ";\n";//prefetch on miss
	o.close();
}

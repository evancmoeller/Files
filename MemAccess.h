#ifndef _MEMACCESS_H
#define _MEMACCESS_H

#include <string>
#include <sstream>
#include <iostream> //hex

class MemAccess
{
public:
	bool store;
	int address;

	MemAccess(std::string s)
	{
		std::string tmp = s.substr(0, 1);

		if(tmp == "S"){store=true;}
		else{store=false;}

		tmp = s.substr(4, 8);
		std::stringstream ss;
		ss << std::hex << tmp;
		ss >> address;
	}
	bool getStore(){return store;}
	int getAddress(){return address;}
};

#endif

#pragma once

#include "common.h"
#include <vector>

class Client;

class Guild
{
public:
	Guild();
	~Guild();

	bool Save();
	bool PromoteUser(string character);
	bool DemoteUser(string character);


	string leader;
	std::vector<string> memberlist;
};


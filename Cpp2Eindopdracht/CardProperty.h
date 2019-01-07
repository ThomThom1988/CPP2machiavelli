#pragma once
#include <string>
//#include "Game.h"

class CardProperty
{
public:
	CardProperty() = default;
	virtual ~CardProperty() = default;
	std::string get_description() { return description; }
	virtual bool useProperty() = 0;
private:	
	bool used;
protected:
	std::string description;
};


#pragma once
#include <string>
//#include "Game.h"

class CardProperty
{
public:
	CardProperty() = default;
	virtual ~CardProperty() = default;
	std::string get_description() const { return description; }
	virtual bool useProperty() = 0;
	bool canUse() const { return canuse; }
	void setCanUse(const bool canUse) { canuse = canUse; }
	std::string getConnectedDescription() { return connectedDescription; }
protected:
	bool canuse;
	std::string description;
	std::string connectedDescription;	
};


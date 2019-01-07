#pragma once
#include "../CardProperty.h"

class DrawCoins :
	public CardProperty
{
public:
	DrawCoins(const int amount) : amount{amount} 
	{ description = "Pak " + std::to_string(amount) + " goudstuk(ken)."; }
	~DrawCoins() {};
	bool useProperty() override { return true; };
private:
	int amount;
};


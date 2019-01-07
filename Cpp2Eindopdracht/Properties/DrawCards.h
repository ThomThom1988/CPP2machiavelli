#pragma once
#include "../CardProperty.h"

class DrawCards :
	public CardProperty
{
public:
	DrawCards(const int amount) : amount{amount} 
	{ description = "Pak " + std::to_string(amount) + " kaart(en) en leg er ��n af."; }
	~DrawCards() {};
	bool useProperty() override { return true; };
private:
	int amount;
};


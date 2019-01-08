#pragma once
#include "../CardProperty.h"
#include "../CharacterCard.h"
#include "../Game.h"

class DrawCoins :
	public CardProperty
{
public:
	DrawCoins(const int amount) : amount{amount} 
	{ description = "Pak " + std::to_string(amount) + " goudstuk(ken)."; }
	~DrawCoins() {};
	bool useProperty() override;
private:
	int amount;
	Game *game;
	CharacterCard *card;
};


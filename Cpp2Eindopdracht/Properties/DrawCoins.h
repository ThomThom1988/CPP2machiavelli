#pragma once
#include "../CardProperty.h"
#include "../CharacterCard.h"
#include "../Game.h"

class DrawCoins :
	public CardProperty
{
public:
	DrawCoins(CharacterCard& characterCard, Game& currentGame, const int amount) : amount{amount}, game{&currentGame}, card{&characterCard}
	{ 
		description = "Pak " + std::to_string(amount) + " goudstuk(ken).";
		canuse = true;
		connectedDescription = amount == 2 ? "Pak 2 kaart(en) en leg er 1 af." : ""; 
	}
	~DrawCoins() {};
	bool useProperty() override;
private:
	int amount;
	Game *game;
	CharacterCard *card;
};


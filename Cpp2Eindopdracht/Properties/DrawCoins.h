#pragma once
#include "../CardProperty.h"
#include "../CharacterCard.h"
#include "../Game.h"

class DrawCoins :
	public CardProperty
{
public:
	DrawCoins(CharacterCard& characterCard, Game& currentGame, const int amount) : amount{amount}, game{&currentGame}, card{&characterCard}
	{ description = "Pak " + std::to_string(amount) + " goudstuk(ken)."; }
	~DrawCoins() {};
	bool useProperty() override;
private:
	int amount;
	Game *game;
	CharacterCard *card;
};


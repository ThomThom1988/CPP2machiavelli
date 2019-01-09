#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class CoinForCard :
	public CardProperty
{
public:
	CoinForCard(Game& currentGame) : game{ &currentGame }
	{
		description = "Leg een bouwkaart af voor een goudstuk."; canuse = true;
	}
	~CoinForCard() {};
	bool useProperty() override;
private:
	Game *game;
};

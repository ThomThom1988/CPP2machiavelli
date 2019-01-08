#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class DrawCards :
	public CardProperty
{
public:
	DrawCards(CharacterCard& characterCard, Game& currentGame ,const int amount, const int discard) : amount{amount} , discardAmount{discard}
	{
		description = "Pak " + std::to_string(amount) + " kaart(en) en leg er "+ std::to_string(discardAmount) +" af.";
		card = &characterCard;
		game = &currentGame;
	}
	~DrawCards() {};
	bool useProperty() override;
private:
	int amount;
	int discardAmount;
	Game *game;
	CharacterCard *card;
};


#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class DrawCards :
	public CardProperty
{
public:
	DrawCards(CharacterCard& characterCard, Game& currentGame ,const int amount, const int discard, const int goldpayment=0) : amount{amount} , discardAmount{discard}, payment{goldpayment}
	{
		description = "Pak " + std::to_string(amount) + " kaart(en) en leg er "+ std::to_string(discardAmount) +" af.";
		card = &characterCard;
		game = &currentGame;
		canuse = true;
		connectedDescription = "Pak 2 goudstuk(ken).";
	}
	~DrawCards() {};
	bool useProperty() override;
private:
	int amount;
	int discardAmount;
	int payment;
	Game *game;
	CharacterCard *card;
};


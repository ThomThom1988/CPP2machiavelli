#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class Steal :
	public CardProperty
{
public:
	Steal(CharacterCard& characterCard, Game& currentGame) : card{ &characterCard }, game{ &currentGame }
	{
		description = "Kies een karakter die je deze ronde wilt bestelen.";
		canuse = true;
	}
	~Steal(){};
	bool useProperty() override;
private:
	CharacterCard *card;
	Game *game;
};


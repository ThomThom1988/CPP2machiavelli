#pragma once
#include "../CardProperty.h"
class Steal :
	public CardProperty
{
public:
	Steal(CharacterCard& characterCard, Game& currentGame)
	{
		description = "Kies een karakter die je deze ronde wilt bestelen.";
		//character = &characterCard;
		//game = &currentGame;
	}
	~Steal(){};
	bool useProperty() override { return true; };
};


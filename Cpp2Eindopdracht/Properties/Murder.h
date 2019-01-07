#pragma once
#include "../CardProperty.h"

class Murder :
	public CardProperty
{
public:
	Murder(CharacterCard& characterCard, Game& currentGame)
	{
		description = "Kies een karakter die je deze ronde wilt vermoorden.";
		character = &characterCard;
		game = &currentGame;
	}
	~Murder(){};
	bool useProperty() override { return true; };
};


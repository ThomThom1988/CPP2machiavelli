#pragma once
#include "../CardProperty.h"
#include "../CharacterCard.h"
#include "../Game.h"

class Murder :
	public CardProperty
{
public:
	Murder(CharacterCard& characterCard, Game& currentGame)
	{
		description = "Kies een karakter die je deze ronde wilt vermoorden.";
		card = &characterCard;
		game = &currentGame;
	}
	~Murder(){};
	bool useProperty() override;
	Game *game;
	CharacterCard *card;
};


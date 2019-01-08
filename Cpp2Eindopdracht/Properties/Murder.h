#pragma once
#include "../CardProperty.h"
#include "../CharacterCard.h"
#include "../Game.h"

class Murder :
	public CardProperty
{
public:
	Murder(CharacterCard& characterCard, Game& currentGame) : card{ &characterCard }, game { &currentGame }
	{
		description = "Kies een karakter die je deze ronde wilt vermoorden.";
	}
	~Murder(){};
	bool useProperty() override;
private:
	CharacterCard *card;
	Game *game;	
};


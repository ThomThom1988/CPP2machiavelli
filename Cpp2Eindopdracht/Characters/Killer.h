#pragma once
#include "Character.h"


class Killer : public Character
{
public:
	Killer(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};


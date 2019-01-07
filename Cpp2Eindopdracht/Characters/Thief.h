#pragma once
#include "Character.h"

class Thief : public Character
{
public:
	Thief(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}

	void setupChoices() override;
private:
};

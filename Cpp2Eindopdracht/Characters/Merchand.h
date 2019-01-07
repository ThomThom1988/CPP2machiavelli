#pragma once
#include "Character.h"

class Merchand : public Character
{
public:
	Merchand(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};

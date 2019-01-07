#pragma once
#include "Character.h"

class King : public Character
{
public:
	King(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};

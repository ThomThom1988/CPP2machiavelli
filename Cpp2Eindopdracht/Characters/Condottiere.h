#pragma once
#include "Character.h"

class Condottiere : public Character
{
public:
	Condottiere(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};

#pragma once
#include "Character.h"

class Magician : public Character
{
public:
	Magician(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};

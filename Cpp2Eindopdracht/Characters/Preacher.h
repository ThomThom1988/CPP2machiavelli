#pragma once
#include "Character.h"

class Preacher : public Character
{
public:
	Preacher(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};

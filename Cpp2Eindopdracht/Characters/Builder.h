#pragma once
#include "Character.h"

class Builder : public Character
{
public:
	Builder(CharacterCard& characterCard, Game& currentGame)
	{
		character = &characterCard;
		game = &currentGame;
	}
	void setupChoices() override;
private:
};
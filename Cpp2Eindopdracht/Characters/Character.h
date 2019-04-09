#pragma once
#include "../Game.h"

class Character
{
public:
	Character() = default;
	virtual ~Character() = default;
	void executeTurn();
	virtual void setupChoices() = 0;
	void addStandardChoices();
	void addPurpleChoices();
	void disableChoice(const std::string description);
	std::string showChoices() const;
protected:
	std::vector<std::unique_ptr<CardProperty>> properties;
	CharacterCard* character;
	Game* game;
};

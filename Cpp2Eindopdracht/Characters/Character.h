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
	friend const Socket & operator<<(Socket & os, Character & c);
protected:
	std::vector<std::unique_ptr<CardProperty>> properties;
	CharacterCard* character;
	Game* game;
};

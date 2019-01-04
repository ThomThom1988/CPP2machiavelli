#pragma once
#include "Card.h"
#include "Player.h"

class CharacterCard :
	public Card
{
public:
	CharacterCard();
	~CharacterCard();

	const Player& get_player() const { return player; }
	const bool& canExecuteBothPowers() const{ return executeBothPowers; }
	const int& maxBuildingsInTurn() const { return amountOfBuildingCards; }
	void set_Player(const Player playerInThisRound) { player = playerInThisRound; };
	friend std::istream & operator>>(std::istream & is, CharacterCard & c);
	friend std::ostream & operator<<(std::ostream & os, CharacterCard & c);

private:
	Player player;
	bool executeBothPowers;
	int amountOfBuildingCards;
};


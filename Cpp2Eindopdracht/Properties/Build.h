#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class Build :
	public CardProperty
{
public:
	Build(CharacterCard& characterCard, Game& currentGame, const int amount) : times{amount}, game{ &currentGame }, card{ &characterCard }
	{
		description = "Bouw een gebouw en betaal de waarde";
	}
	~Build() {};
	bool useProperty() override;
	void setTimes(const int amountOfTimes) { times = amountOfTimes; }
private:
	int times;
	Game *game;
	CharacterCard *card;
};

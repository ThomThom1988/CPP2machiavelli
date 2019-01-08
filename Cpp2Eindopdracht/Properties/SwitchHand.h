#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class SwitchHand :
	public CardProperty
{
public:
	SwitchHand(CharacterCard& characterCard, Game& currentGame) : card{ &characterCard }, game{ &currentGame }
	{ description = "Wissel van hand met jouw tegenstander"; }
	~SwitchHand() {};
	bool useProperty() override;
private:
	CharacterCard *card;
	Game *game;
};

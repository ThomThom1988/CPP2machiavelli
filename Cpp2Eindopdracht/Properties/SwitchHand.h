#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class SwitchHand :
	public CardProperty
{
public:
	SwitchHand(CharacterCard& characterCard, Game& currentGame) : card{ &characterCard }, game{ &currentGame }
	{ 
		description = "Wissel van hand met jouw tegenstander"; 
		canuse = true;
		connectedDescription = "Wissel een aantal handkaarten om voor nieuwe gebouwenkaarten.";
	}
	~SwitchHand() {};
	bool useProperty() override;
private:
	CharacterCard *card;
	Game *game;
};

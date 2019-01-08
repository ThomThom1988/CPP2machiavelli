#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class SwitchCards :
	public CardProperty
{
public:
	SwitchCards(Game& currentGame) : game{ &currentGame } { description = "Wissel een aantal handkaarten om voor nieuwe gebouwenkaarten."; }
	~SwitchCards() {};
	bool useProperty() override;
private:
	Game *game;
};

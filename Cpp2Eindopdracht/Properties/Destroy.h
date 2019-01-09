#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class Destroy :
	public CardProperty
{
public:
	Destroy(Game& currentGame) : game{ &currentGame }
	{ description = "Vernietig een gebouw van de tegenstander voor 1 goudstuk minder dan de waarde."; canuse = true;
	}
	~Destroy() {};
	bool useProperty() override;
private:
	Game *game;
};

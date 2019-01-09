#pragma once
#include "../CardProperty.h"
#include "../Game.h"

class ColorBonus :
	public CardProperty
{
public:
	ColorBonus(Game& currentGame,std::string cardcolor) : color{cardcolor}, game{&currentGame}
	{ description = "Ontvang 1 goudstuk voor ieder gebouw van de kleur " + color + "."; canuse = true; }
	~ColorBonus() {};
	bool useProperty() override;
private:
	std::string color;
	Game* game;
};

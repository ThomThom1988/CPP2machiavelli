#pragma once
#include "../CardProperty.h"

class ColorBonus :
	public CardProperty
{
public:
	ColorBonus(std::string cardcolor) : color{cardcolor} 
	{ description = "Ontvang 1 goudstuk voor ieder gebouw van de kleur " + color + "."; }
	~ColorBonus() {};
	bool useProperty() override { return true; };
private:
	std::string color;
};

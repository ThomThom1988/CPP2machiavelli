#pragma once
#include "../CardProperty.h"

class Destroy :
	public CardProperty
{
public:
	Destroy() 
	{ description = "Vernietig een gebouw van de tegenstander voor 1 goudstuk minder dan de waarde."; }
	~Destroy() {};
	bool useProperty() override { return true; };
};
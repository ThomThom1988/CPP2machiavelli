#pragma once
#include "../CardProperty.h"

class Murder :
	public CardProperty
{
public:
	Murder() { description = "Kies een karakter die je deze ronde wilt vermoorden."; }
	~Murder(){};
	bool useProperty() override { return true; };
};


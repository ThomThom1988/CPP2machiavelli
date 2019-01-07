#pragma once
#include "../CardProperty.h"
class Thief :
	public CardProperty
{
public:
	Thief();
	~Thief();
	bool useProperty() override { return true; };
};


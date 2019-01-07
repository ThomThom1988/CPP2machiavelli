#pragma once
#include "../CardProperty.h"

class SwitchHand :
	public CardProperty
{
public:
	SwitchHand() { description = "Wissel een aantal handkaarten om voor nieuwe gebouwenkaarten."; }
	~SwitchHand() {};
	bool useProperty() override { return true; };
};

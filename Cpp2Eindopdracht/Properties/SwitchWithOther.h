#pragma once
#include "../CardProperty.h"

class SwitchWithOther :
	public CardProperty
{
public:
	SwitchWithOther() { description = "Wissel van hand met jouw tegenstander"; }
	~SwitchWithOther() {};
	bool useProperty() override { return true; };
};

#pragma once
#include "../CardProperty.h"

class Build :
	public CardProperty
{
public:
	Build() : times{1}
	{
		description = "Bouw een gebouw en betaal de waarde";
	}
	~Build() {};
	bool useProperty() override { return true; };
	void setTimes(const int amountOfTimes) { times = amountOfTimes; }
private:
	int times;
};

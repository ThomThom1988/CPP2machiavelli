#include "King.h"
#include "../Properties/ColorBonus.h"

void King::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<ColorBonus>(*game,"geel"));
}

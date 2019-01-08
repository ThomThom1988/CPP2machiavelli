#include "Preacher.h"
#include "../Properties/ColorBonus.h"

void Preacher::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<ColorBonus>(*game, "blauw"));
}

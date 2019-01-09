#include "Merchand.h"
#include "../Properties/ColorBonus.h"
#include "../Properties/DrawCoins.h"

void Merchand::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<DrawCoins>(*character, *game, 1));
	properties.push_back(std::make_unique<ColorBonus>(*game, "groen"));
}

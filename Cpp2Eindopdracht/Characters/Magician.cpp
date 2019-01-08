#include "Magician.h"
#include "../Properties/SwitchHand.h"
#include "../Properties/SwitchCards.h"

void Magician::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<SwitchHand>(*character, *game));
	properties.push_back(std::make_unique<SwitchCards>(*game));
}

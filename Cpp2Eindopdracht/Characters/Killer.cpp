#include "Killer.h"
#include "../Properties/Murder.h"

void Killer::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<Murder>(*character, *game));
}

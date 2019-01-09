#include "Thief.h"
#include "../Properties/Steal.h"

void Thief::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<Steal>(*character, *game));
}

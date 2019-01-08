#include "Condottiere.h"
#include "../Properties/Destroy.h"

void Condottiere::setupChoices()
{
	addStandardChoices();
	properties.push_back(std::make_unique<Destroy>(*game));
}

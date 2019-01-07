#include "Killer.h"
#include "../Properties/Murder.h"

void Killer::setupChoices()
{
	properties.push_back(std::make_unique<Murder>(*character, *game));
}

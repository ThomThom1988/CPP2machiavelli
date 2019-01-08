#include "Builder.h"
#include "../Properties/DrawCards.h"
#include "../Properties/DrawCoins.h"
#include "../Properties/Build.h"

void Builder::setupChoices()
{
	properties.push_back(std::make_unique<DrawCards>(*character, *game, 2, 1));
	properties.push_back(std::make_unique<DrawCoins>(*character, *game, 2));
	properties.push_back(std::make_unique<Build>(*character, *game, 3));
	properties.push_back(std::make_unique<DrawCards>(*character, *game, 2, 0));
}

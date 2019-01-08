#include "SwitchCards.h"

bool SwitchCards::useProperty()
{
	std::vector<std::unique_ptr<BuildingCard>> cards = std::move(game->getCurrentPlayer()->discardCards());
	for(auto &x : cards)
	{
		game->addBuildingToDraw(std::move(x));
	}
	game->drawCards(cards.size(), 0, game->getCurrentPlayer());
	setCanUse(false);
	return true;
}

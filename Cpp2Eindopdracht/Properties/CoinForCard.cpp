#include "CoinForCard.h"

bool CoinForCard::useProperty()
{
	std::unique_ptr<BuildingCard> card = std::move(game->getCurrentPlayer()->sellCard());
	if (card != nullptr) {
		game->addBuildingToDraw(std::move(card));
		setCanUse(false);
	}
	return true;
}

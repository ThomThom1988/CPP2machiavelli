#include "SwitchHand.h"

bool SwitchHand::useProperty()
{
	std::vector<std::unique_ptr<BuildingCard>> hand;
	std::vector<std::unique_ptr<BuildingCard>> otherhand;
	hand = std::move(game->getCurrentPlayer()->get_hand());
	otherhand = std::move(game->getOtherPlayer(game->getCurrentPlayer())->get_hand());
	game->getCurrentPlayer()->set_hand(std::move(otherhand));
	game->getOtherPlayer(game->getCurrentPlayer())->set_hand(std::move(hand));
	setCanUse(false);
	return true;
}

#include "Destroy.h"

bool Destroy::useProperty()
{
	if(game->canDestroyBuildings(game->getOtherPlayer(game->getCurrentPlayer())))
	{
		std::unique_ptr<BuildingCard> card =
			std::move(game->getOtherPlayer(game->getCurrentPlayer())->destroyBuilding(game->getCurrentPlayer()));
		game->addBuildingToDraw(std::move(card));
		setCanUse(false);
	}
	else game->getCurrentPlayer()->get_socket() << "Je kunt niets vernietigen van de prediker.\r\n";
	return true;
}

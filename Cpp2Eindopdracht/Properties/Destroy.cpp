#include "Destroy.h"

bool Destroy::useProperty()
{
	if(game->getCurrentPlayer()->getAmountOfBuildings() >= 8) game->getCurrentPlayer()->get_socket() << "\r\nDe stad van de tegenstander is vol. Er kan niets worden vernietigd.\r\n";
	else if(game->canDestroyBuildings(game->getOtherPlayer(game->getCurrentPlayer())))
	{
		std::unique_ptr<BuildingCard> card =
			std::move(game->getOtherPlayer(game->getCurrentPlayer())->destroyBuilding(game->getCurrentPlayer()));
		game->addBuildingToDraw(std::move(card));
		setCanUse(false);
	}
	else game->getCurrentPlayer()->get_socket() << "r\nJe kunt niets vernietigen van de prediker.\r\n";
	return true;
}

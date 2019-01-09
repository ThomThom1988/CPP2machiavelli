#include "DrawCards.h"

bool DrawCards::useProperty()
{
	if (!game->inCheatMode()) setCanUse(false);
	if (payment > 0) game->getCurrentPlayer()->get_player().addGold(-payment);
	return game->drawCards(amount, discardAmount, card->get_player());
}

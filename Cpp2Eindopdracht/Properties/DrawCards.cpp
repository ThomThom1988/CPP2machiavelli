#include "DrawCards.h"

bool DrawCards::useProperty()
{
	setCanUse(false);
	if (payment > 0) game->getCurrentPlayer()->get_player().addGold(-payment);
	return game->drawCards(amount, discardAmount, card->get_player());
}

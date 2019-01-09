#include "DrawCards.h"

bool DrawCards::useProperty()
{
	setCanUse(false);
	return game->drawCards(amount, discardAmount, card->get_player());
}

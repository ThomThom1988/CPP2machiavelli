#include "DrawCards.h"

bool DrawCards::useProperty()
{
	return game->drawCards(amount, discardAmount, card->get_player());
}

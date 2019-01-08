#include "ColorBonus.h"

bool ColorBonus::useProperty()
{
	int amount = game->getCurrentPlayer()->getColorBonus(color);
	if ( amount > 0) game->getCurrentPlayer()->get_socket() << "Je hebt " << std::to_string(amount) << " goudstuk(ken) erbij gekregen.\r\n";
	else game->getCurrentPlayer()->get_socket() << "Je hebt geen gebouwen van de kleur " << color << ".\r\n";
	return true;
}

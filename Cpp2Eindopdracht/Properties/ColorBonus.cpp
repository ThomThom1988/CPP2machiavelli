#include "ColorBonus.h"

bool ColorBonus::useProperty()
{
	int amount = game->getCurrentPlayer()->getColorBonus(color);
	if (amount > 0) {
		game->getCurrentPlayer()->get_socket() << "\r\nJe hebt " << std::to_string(amount) << " goudstuk(ken) erbij gekregen.\r\n";
		setCanUse(false);
	}
	else game->getCurrentPlayer()->get_socket() << "\r\nJe hebt geen gebouwen van de kleur " << color << ".\r\n";
	return true;
}

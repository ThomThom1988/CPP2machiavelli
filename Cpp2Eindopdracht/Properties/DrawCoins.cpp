#include "DrawCoins.h"

bool DrawCoins::useProperty()
{
	card->get_player()->get_player().addGold(amount);
	card->get_player()->get_socket() << "\r\nJe hebt " << std::to_string(amount) << " goudstukken gekregen.\r\n";
	if (!game->inCheatMode()) setCanUse(false);
	return true;
}

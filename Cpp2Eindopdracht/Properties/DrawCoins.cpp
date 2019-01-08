#include "DrawCoins.h"

bool DrawCoins::useProperty()
{
	card->get_player()->get_player().addGold(amount);
	card->get_player()->get_socket() << "Je hebt " << std::to_string(amount) << "goudstukken gekregen.\r\n";
	return true;
}

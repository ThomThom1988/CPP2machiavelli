#include "ClientInfo.h"
#include "Game.h"
#include <algorithm>
#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

bool ClientInfo::addBuilding(int index)
{
	try
	{
		BuildingCard* cardpointer = hand.at(index).get();
		if (cardpointer->get_value() > _player.get_gold()) return false;
		_player.addGold(-cardpointer->get_value());
		std::unique_ptr<BuildingCard> card = std::move(hand.at(index));
		buildings.push_back(std::move(card));
		auto e = std::find(hand.begin(), hand.end(), nullptr);
		if (e != hand.end()) {
			hand.erase(e);
		}
		
		return true;
	}
	catch (...)
	{
		return false;
	}	
}

void ClientInfo::printInfo()
{
	printGold(_socket);
	printBuildings(_socket);
	printHand();
}

void ClientInfo::printHand()
{
	_socket << "\r\nKaarten in hand:\r\n";
	if (hand.empty()) _socket << "Je hebt geen kaarten in jouw hand.\r\n";
	else {
		int index = 0;
		for (auto &x : hand) {
			if (x != nullptr) {
				_socket << "[" << std::to_string(index) << "] ";
				_socket << *x.get() << "\r\n";
			}
			index++;
		}
	}
}

void ClientInfo::printBuildings(Socket& socket)
{
	socket << "\r\nGebouwen:\r\n";
	if (buildings.empty()) socket << "Er zijn geen gebouwen geplaatst.\r\n";
	else for (auto &x : buildings) socket << *x.get() << "\r\n";
}

void ClientInfo::printGold(Socket& socket)
{
	socket << "\r\nGoudstukken:" << _player.get_gold() << ".\r\n";
}

int ClientInfo::getColorBonus(const std::string color)
{
	int bonus = 0;
	for(auto &x : buildings) if(x->get_color() == color) bonus++;
	if (bonus > 0) get_player().addGold(bonus);
	return bonus;
}

std::vector<std::unique_ptr<BuildingCard>> ClientInfo::discardCards()
{
	std::vector<std::unique_ptr<BuildingCard>> cards;
	int amount = 0;
	bool stop{ false };
	while (!stop)
	{
		_socket << "\r\nkies een kaart om af te leggen.\r\n";
		int index = 0;
		for (auto &x : hand) {
			if (x != nullptr) {
				_socket << "[" << std::to_string(index) << "] ";
				_socket << *x.get() << "\r\n";
			}
			index++;
		}
		_socket << "[stop] Leg geen kaarten meer neer en pak " << std::to_string(amount) << "nieuwe kaarten.\r\n";
		_socket << machiavelli::prompt;
		int chosenIndex;
		bool done = false;
		std::string chosenIndexString;
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = _socket.readline([&chosenIndexString](std::string input) {
					chosenIndexString = input;
				});
			}
			if(chosenIndexString != "stop")
			{
				try
				{
					chosenIndex = std::stoi(chosenIndexString);
					std::unique_ptr<BuildingCard> choice{ std::move(hand.at(chosenIndex)) };
					cards.push_back(std::move(choice));
					auto e = std::find(hand.begin(), hand.end(), nullptr);
					if (e != hand.end()) {
						hand.erase(e);
					}
					amount++;
					done = true;
				}
				catch (...)
				{
					_socket << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
				}
			}
			else {
				done = true; stop = true;
			}
		}
	}
	return std::move(cards);
}

std::unique_ptr<BuildingCard> ClientInfo::sellCard()
{
	std::unique_ptr<BuildingCard> card;
	bool stop{ false };
	while (!stop)
	{
		_socket << "\r\nkies een kaart om af te leggen.\r\n";
		int index = 0;
		for (auto &x : hand) {
			if (x != nullptr) {
				_socket << "[" << std::to_string(index) << "] ";
				_socket << *x.get() << "\r\n";
			}
			index++;
		}
		_socket << "[stop] terug naar keuze.\r\n";
		_socket << machiavelli::prompt;
		int chosenIndex;
		bool done = false;
		std::string chosenIndexString;
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = _socket.readline([&chosenIndexString](std::string input) {
					chosenIndexString = input;
				});
			}
			if (chosenIndexString != "stop")
			{
				try
				{
					chosenIndex = std::stoi(chosenIndexString);
					card = std::move(hand.at(chosenIndex));
					auto e = std::find(hand.begin(), hand.end(), nullptr);
					if (e != hand.end()) {
						hand.erase(e);
					}
					done = true;
					stop = true;
					_player.addGold(1);
				}
				catch (...)
				{
					_socket << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
				}
			}
			else {
				done = true; stop = true;
			}
		}
	}
	return std::move(card);
}

std::unique_ptr<BuildingCard> ClientInfo::destroyBuilding(std::shared_ptr<ClientInfo> player)
{
	std::unique_ptr<BuildingCard> card;
	bool graveyardUsed{ false };
	bool stop{ false };
	while (!stop)
	{
		player->get_socket() << "\r\nkies een kaart om te vernietigen.\r\n";
		int index = 0;
		for (auto &x : buildings) {
			if (x != nullptr) {
				player->get_socket() << "[" << std::to_string(index) << "] ";
				player->get_socket() << *x.get() << "\r\n";
			}
			index++;
		}
		player->get_socket() << "[stop] Ga terug naar keuze.\r\n";
		player->get_socket() << machiavelli::prompt;
		int chosenIndex;
		bool done = false;
		std::string chosenIndexString;
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = player->get_socket().readline([&chosenIndexString](std::string input) {
					chosenIndexString = input;
				});
			}
			if (chosenIndexString != "stop")
			{
				try
				{
					chosenIndex = std::stoi(chosenIndexString);
					if(player->get_player().get_gold() < (buildings.at(chosenIndex)->get_value() -1))
					{
						player->get_socket() << "\r\nje hebt niet genoeg goudstukken om deze kaart te vernietigen.\r\n";
					}
					else if (buildings.at(chosenIndex)->get_name() == "Kerker")
					{
						player->get_socket() << "\r\nje mag de kerker niet vernietigen.\r\n";
					}
					else
					{
						_socket << "\r\nJouw " << buildings.at(chosenIndex)->get_name() << " wordt verwoest door de condottiere, ";
						//kerkhof implementatie
						if (_player.get_gold() > 0)
						{
							std::vector<std::unique_ptr<BuildingCard>>::iterator graveyard =
								std::find_if(hand.begin(), hand.end(),
									[&](std::unique_ptr<BuildingCard> & obj) { return obj->get_name() == "Kerkhof"; }
							);

							if(graveyard != hand.end())
							{
								std::string yesorno;
								_socket << "wil je deze voor 1 goudstuk terug in jouw hand steken?[y/n]\r\n" << machiavelli::prompt;
								player->get_socket() << "\r\nDe tegenstander heeft de kans om het gebouw terug in de hand te stoppen.\r\n";								
								bool yesornochoice{ false };
								while(!yesornochoice)
								{
									inputgotten = false;
									while (!inputgotten) {
										inputgotten = _socket.readline([&yesorno](std::string input) {
											yesorno = input;
										});
									}
									if (yesorno == "y" || yesorno == "n") yesornochoice = true;
								}
								if (yesorno == "y")
								{
									graveyardUsed = true;
									player->get_socket() << "\r\nDe tegenstander heeft de kaart teruggepakt.\r\n";
								}
								else player->get_socket() << "\r\nDe tegenstander heeft de kaart niet teruggepakt.\r\n";
							}

						}
						card = std::move(buildings.at(chosenIndex));
						auto e = std::find(buildings.begin(), buildings.end(), nullptr);
						if (e != buildings.end()) {
							buildings.erase(e);
						}
						done = true;
						stop = true;
						player->get_player().addGold(-(card->get_value()-1));
						//graveyard part 2
						if (graveyardUsed)
						{
							addCard(std::move(card));
						}
					}					
				}
				catch (...)
				{
					player->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
				}
			}
			else {
				done = true; stop = true;
			}
		}
	}
	return std::move(card);
}

#include "ClientInfo.h"
#include "Game.h"

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
	_socket << "Kaarten in hand:\r\n";
	if (hand.empty()) _socket << "Je hebt geen kaarten in jouw hand.\r\n";
	else for (auto &x : hand) if(x!=nullptr)_socket << *x.get() << "\r\n";
}

void ClientInfo::printBuildings(Socket& socket)
{
	socket << "Gebouwen:\r\n";
	if (buildings.empty()) socket << "Er zijn geen gebouwen geplaatst.\r\n";
	else for (auto &x : buildings) socket << *x.get() << "\r\n";
}

void ClientInfo::printGold(Socket& socket)
{
	socket << "Goudstukken:" << _player.get_gold() << ".\r\n";
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
		_socket << "kies een kaart om af te leggen.\r\n";
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
					_socket << "kies een juiste waarde.\r\n" << machiavelli::prompt;
				}
			}
			else {
				done = true; stop = true;
			}
		}
	}
	return std::move(cards);
}

std::unique_ptr<BuildingCard> ClientInfo::destroyBuilding(std::shared_ptr<ClientInfo> player)
{
	std::unique_ptr<BuildingCard> card;
	bool stop{ false };
	while (!stop)
	{
		player->get_socket() << "kies een kaart om te vernietigen.\r\n";
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
						player->get_socket() << "je hebt niet genoeg goudstukken om deze kaart te vernietigen.\r\n";
					}
					else
					{
						card = std::move(buildings.at(chosenIndex));
						auto e = std::find(buildings.begin(), buildings.end(), nullptr);
						if (e != buildings.end()) {
							buildings.erase(e);
						}
						done = true;
						stop = true;
						player->get_player().addGold(-(card->get_value()-1));
					}					
				}
				catch (...)
				{
					player->get_socket() << "kies een juiste waarde.\r\n" << machiavelli::prompt;
				}
			}
			else {
				done = true; stop = true;
			}
		}
	}
	return std::move(card);
}

#include "ClientInfo.h"

bool ClientInfo::addBuilding(std::unique_ptr<BuildingCard> card)
{
	if (card->get_value() > _player.get_gold()) return false;
	buildings.push_back(std::move(card));
	return true;
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
	else for (auto &x : hand) _socket << *x.get() << "\r\n";
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

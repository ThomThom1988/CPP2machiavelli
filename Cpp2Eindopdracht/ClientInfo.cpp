#include "ClientInfo.h"

bool ClientInfo::addBuilding(std::unique_ptr<Card> card)
{
	if (card->get_value() > _player.get_gold()) return false;
	buildings.push_back(std::move(card));
	return true;
}

void ClientInfo::printInfo()
{
	printGold();
	printBuildings();
	printHand();
	//printOptions();
}

void ClientInfo::printHand()
{
	_socket << "Kaarten in hand:\r\n";
	if (hand.empty()) _socket << "Je hebt geen kaarten in jouw hand.\r\n";
	//else for (auto &x : hand) _socket << x.get() << "\r\n";
}

void ClientInfo::printBuildings()
{
	_socket << "Gebouwen:\r\n";
	if (buildings.empty()) _socket << "Je hebt nog geen gebouwen geplaatst.\r\n";
	//else for (auto &x : hand) _socket << x.get() << "\r\n";
}

//void ClientInfo::printOptions()
//{
//}

void ClientInfo::printGold()
{
	_socket << "Goudstukken:" << _player.get_gold() << ".\r\n";
}

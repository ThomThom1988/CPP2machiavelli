#include "Game.h"
#include <algorithm>
#include "Fileparser.h"
#include <random>
#include "CharacterCard.h"
#include <iterator>

Game::~Game()
{
}

std::shared_ptr<ClientInfo> Game::getOtherPlayer(const std::shared_ptr<ClientInfo> player)
{
	ptrdiff_t pos = std::distance(players.begin(), std::find(players.begin(), players.end(), player));
	if (pos == 1) return players[0];
	return players[1];
}

void Game::changePlayer()
{
	players.at(currentPlayer)->get_socket().write("Je tegenstander is nu aan de beurt.\r\n");
	currentPlayer = currentPlayer == 0 ? 1 : 0;
}

void Game::startGame()
{
	Fileparser parser;
	for (auto &x : players) {
		
		auto other = getOtherPlayer(x);
		x->get_socket().write("Je speelt het spel tegen: " + other->get_player().get_name() + "\r\n");
	}
	king = players[0]->get_player().get_age() >= players[1]->get_player().get_age() ? 0 : 1;
	printKingInfo();
	for (auto &x : players) drawCards(4, x);
	//roundSetup();
	cheatSetup();
	resetCharacters();
}

void Game::roundSetup()
{
	currentRound++;
	currentPlayer = king;
	std::shuffle(drawBuildings.begin(), drawBuildings.end(), std::default_random_engine{});
	std::shuffle(drawCharacters.begin(), drawCharacters.end(), std::default_random_engine{});
	discardCharacter(drawCharacters[0]->get_value());
	players[king]->get_socket().write("kies een karakter");
}

void Game::cheatSetup()
{
	currentRound++;
	chooseCharacter(1, players.at(0));
	chooseCharacter(2, players.at(0));
	chooseCharacter(3, players.at(1));
	chooseCharacter(4, players.at(1));
	discardCharacter(5);
	discardCharacter(6);
	discardCharacter(7);
	discardCharacter(8);	
}

void Game::printKingInfo()
{
	auto kingsname = players.at(king)->get_player().get_name();
	players.at(king)->get_socket().write("Je bent de koning.\r\n");
	auto other = getOtherPlayer(players.at(king));
	other->get_socket().write(kingsname + " is de koning.\r\n");
}

void Game::printOtherInfo(const std::shared_ptr<ClientInfo> asker)
{
	auto other = getOtherPlayer(asker);
	asker->get_socket() << "Informatie over " << other->get_player().get_name() << ": \r\n"
	<< "Goudstukken: " << other->get_player().get_gold() << ". \r\n" << "Gebouwen: \r\n";
	auto buildings = other->get_buildings();
	if (buildings.empty()) asker->get_socket() << "Je tegenstander heeft nog geen gebouwen.\r\n";
	else for (auto &x : buildings) asker->get_socket() << x.get() << "\r\n";
}

bool Game::drawCards(const int amount, const std::shared_ptr<ClientInfo> player)
{
	for (int i = 0; i < amount; i++)
	{
		if (drawBuildings.front() != nullptr)
		{
			player->addCard(std::move(drawBuildings.front()));
			drawBuildings.erase(drawBuildings.begin());
		}
		else return false;
		
	}
	return true;
}

bool Game::chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player)
{
	std::vector<std::unique_ptr<Card>>::iterator object =
		std::find_if(drawCharacters.begin(), drawCharacters.end(),
			[&](std::unique_ptr<Card> & obj) { return obj->get_value() == character; }
	);
	CharacterCard *derivedPointer = dynamic_cast<CharacterCard*>(object->get());
	derivedPointer->set_Player(player->get_player());
	addCharacterToDiscard(std::move(*object));
	drawCharacters.erase(std::remove(drawCharacters.begin(), drawCharacters.end(), *object));
	return true;
}

bool Game::discardCharacter(const int discardCharacter)
{
	std::vector<std::unique_ptr<Card>>::iterator object =
		std::find_if(drawCharacters.begin(), drawCharacters.end(),
			[&](std::unique_ptr<Card> & obj) { return obj->get_value() == discardCharacter; }
	);
	addCharacterToDiscard(std::move(*object));
	drawCharacters.erase(std::remove(drawCharacters.begin(), drawCharacters.end(), *object));
	return true;
}

void Game::resetCharacters()
{
	for (auto &x :discardedCharacters)
	{
		CharacterCard *derivedPointer = dynamic_cast<CharacterCard*>(x.get());
		derivedPointer->reset_player();
	}
	std::move(begin(discardedCharacters), end(discardedCharacters), std::inserter(drawCharacters, end(drawCharacters)));
	discardedCharacters.clear();
}

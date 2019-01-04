#include "Game.h"
#include <algorithm>
#include "Fileparser.h"
#include <random>
#include "CharacterCard.h"

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
	if (currentPlayer == 0) currentPlayer = 1;
	else currentPlayer = 0;
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
	std::shuffle(drawBuildings.begin(), drawBuildings.end(), std::default_random_engine{});
	std::shuffle(drawCharacters.begin(), drawCharacters.end(), std::default_random_engine{});
	roundSetup();
}

void Game::roundSetup()
{
	currentRound++;
	currentPlayer = king;
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

bool Game::drawCards(const int amount, const std::shared_ptr<ClientInfo> player)
{
	return false;
}

bool Game::chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player)
{
	std::vector<std::unique_ptr<Card>>::iterator object =
		std::find_if(drawCharacters.begin(), drawCharacters.end(),
			[&](std::unique_ptr<Card> & obj) { return obj->get_value() == character; }
	);
	CharacterCard *derivedPointer = dynamic_cast<CharacterCard*>(object->get());
	derivedPointer->set_Player(player->get_player());
	drawCharacters.erase(std::remove(drawCharacters.begin(), drawCharacters.end(), *object));
	addCharacterToDiscard(std::move(*object));	
	return true;
}

bool Game::discardCharacter(const int discardCharacter)
{
	std::vector<std::unique_ptr<Card>>::iterator object =
		std::find_if(drawCharacters.begin(), drawCharacters.end(),
			[&](std::unique_ptr<Card> & obj) { return obj->get_value() == discardCharacter; }
	);
	drawCharacters.erase(std::remove(drawCharacters.begin(), drawCharacters.end(), *object));
	addCharacterToDiscard(std::move(*object));
	return true;
}

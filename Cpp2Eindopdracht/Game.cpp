#include "Game.h"
#include <algorithm>
#include "Fileparser.h"
#include <random>
#include "CharacterCard.h"
#include <iterator>
#include "Turn.h"

Game::~Game()
{
}

std::shared_ptr<ClientInfo> Game::getOtherPlayer(const std::shared_ptr<ClientInfo> player) const
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

bool Game::allCharactersChosen()
{
	bool result = true;
	for (auto &character : characters) if (!character->is_discarded()) result = false;
	return result;
}

void Game::roundSetup()
{
	currentRound++;
	currentPlayer = king;
	std::shuffle(buildings.begin(), buildings.end(), std::default_random_engine{});
	std::shuffle(characters.begin(), characters.end(), std::default_random_engine{});
	discardCharacter(characters[0]->get_value());

	//karakter keuzes hier
	while (!allCharactersChosen())
	{
		players[currentPlayer]->get_socket().write("kies een karakter: \r\n");
		showCharacterChoices();
		int choice;
		bool done{ false };
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = players[currentPlayer]->get_socket().readline([&choice,this](std::string input) {
					try	{choice = std::stoi(input);	}
					catch (...){players[currentPlayer]->get_socket() << "kies een juiste waarde." << machiavelli::prompt;}					
				});
			}
			done = chooseCharacter(choice, players[currentPlayer]);
			if(!done) players[currentPlayer]->get_socket() << "kies een juiste waarde." << machiavelli::prompt;
		}	

		if (!allCharactersChosen())
		{
			players[currentPlayer]->get_socket().write("kies een karakter om weg te leggen: \r\n");
			showCharacterChoices();
			done = false;
			while (!done) {
				bool inputgotten{ false };
				while (!inputgotten) {
					inputgotten = players[currentPlayer]->get_socket().readline([&choice, this](std::string input) {
						try { choice = std::stoi(input); }
						catch (...) { players[currentPlayer]->get_socket() << "kies een juiste waarde.\r\n" << machiavelli::prompt; }
					});
				}
				done = discardCharacter(choice);
				if (!done) players[currentPlayer]->get_socket() << "kies een juiste waarde.\r\n" << machiavelli::prompt;
			}
			changePlayer();
		}		
	}
	startRound();
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
	startRound();
}

void Game::startTurn(std::string character)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_name() == character; }
	);
	if (object->get() == nullptr || object->get()->get_name() == killedCharacter)
	{
		for (auto &x : players) x->get_socket() << "De " << character << "komt deze ronde niet aan de beurt.\r\n";
	}
	else
	{
		Turn turn(*object->get());

	}
}

void Game::startRound()
{
	startTurn("Moordenaar");
	startTurn("Dief");
	startTurn("Magiër");
	startTurn("Koning");
	startTurn("Prediker");
	startTurn("Koopman");
	startTurn("Bouwmeester");
	startTurn("Condottière");
	if (gameEnded)
	{
		endGame();
	}
}

void Game::endGame()
{
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
	//auto buildings = other->get_buildings();
	//if (buildings.empty()) asker->get_socket() << "Je tegenstander heeft nog geen gebouwen.\r\n";
	//else for (auto &x : buildings) asker->get_socket() << x.get() << "\r\n";
}

bool Game::drawCards(const int amount, const std::shared_ptr<ClientInfo> player)
{
	for (int i = 0; i < amount; i++)
	{
		if (buildings.front() != nullptr)
		{
			player->addCard(std::move(buildings.front()));
			buildings.erase(buildings.begin());
		}
		else return false;
		
	}
	return true;
}

bool Game::chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_value() == character; }
	);
	if (object->get() == nullptr) return false;
	object->get()->set_Player(player);
	object->get()->set_discarded(true);
	return true;
}

bool Game::discardCharacter(const int discardCharacter)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_value() == discardCharacter; }
	);
	object->get()->set_discarded(true);
	return true;
}

void Game::resetCharacters()
{
	for (auto &character :characters)
	{
		character->reset_player();
		character->set_discarded(false);
	}
}

void Game::showCharacterChoices()
{
	for(auto &character : characters)
	{
		if (!character->is_discarded())
		{
			players[currentPlayer]->get_socket() << *character << "\r\n";
		}		
	}
	players[currentPlayer]->get_socket() << machiavelli::prompt;
}

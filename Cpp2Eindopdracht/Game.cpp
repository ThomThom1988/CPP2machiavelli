#include "Game.h"
#include <algorithm>
#include <random>
#include <iterator>
#include "Characters/Killer.h"
#include "Characters/Thief.h"
#include "Characters/Magician.h"
#include "Characters/King.h"
#include "Characters/Preacher.h"
#include "Characters/Merchand.h"
#include "Characters/Builder.h"
#include "Characters/Condottiere.h"

Game::~Game()
{
}

std::shared_ptr<ClientInfo> Game::getOtherPlayer(std::shared_ptr<ClientInfo> player) const
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
	std::shuffle(buildings.begin(), buildings.end(), std::default_random_engine{});
	for (auto &x : players) {
		
		auto other = getOtherPlayer(x);
		x->get_socket().write("Je speelt het spel tegen: " + other->get_player().get_name() + "\r\n");
		drawCards(4, 0, x);
		x->get_player().addGold(2);
		currentPlayer = currentPlayer == 0 ? 1 : 0;
	}
	king = players[0]->get_player().get_age() >= players[1]->get_player().get_age() ? 0 : 1;
	printKingInfo();
	//roundSetup();
	cheatSetup();
	resetCharacters();
}

bool Game::allCharactersChosen()
{
	bool result = true;
	CharacterCard card;
	auto a = card.get_name();
	for (auto &character : characters) if (!character->is_discarded()) result = false;
	return result;
}

void Game::roundSetup()
{
	currentRound++;
	currentPlayer = king;
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
	currentPlayer = king;
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
	if (object->get()->get_player() == nullptr || character == killedCharacter)
	{
		for (auto &x : players) x->get_socket() << "De " << character << "komt deze ronde niet aan de beurt.\r\n";
	}
	else
	{
		if (object->get()->get_name() == goldStolenCharacter)
		{
			std::vector<std::unique_ptr<CharacterCard>>::iterator dief =
				std::find_if(characters.begin(), characters.end(),
					[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_name() == "Dief"; }
			);
			int amount = object->get()->get_player()->get_player().get_gold();
			dief->get()->get_player()->get_player().addGold(amount);
			object->get()->get_player()->get_player().addGold(-amount);

			dief->get()->get_player()->get_socket() << "Je hebt " << std::to_string(amount) <<" goudstukken gestolen van de " << character << "!\r\n";
			object->get()->get_player()->get_socket() << "Al jouw goudstukken zijn gestolen door de dief\r\n";
		}
		if (object->get()->get_player() != players[currentPlayer]) changePlayer();
		std::unique_ptr<Character> turn;
		//characters
		switch (object->get()->get_value())
		{
		case 1:
			turn = std::make_unique<Killer>(*object->get(), *this);
			break;
		case 2:
			turn = std::make_unique<Thief>(*object->get(), *this);
			break;
		case 3:
			turn = std::make_unique<Magician>(*object->get(), *this);
			break;
		case 4:
			turn = std::make_unique<King>(*object->get(), *this);
			king = currentPlayer;
			break;
		case 5:
			turn = std::make_unique<Preacher>(*object->get(), *this);
			break;
		case 6:
			turn = std::make_unique<Merchand>(*object->get(), *this);
			break;
		case 7:
			turn = std::make_unique<Builder>(*object->get(), *this);
			break;
		case 8:
			turn = std::make_unique<Condottiere>(*object->get(), *this);
			break;
		default:
			turn = std::make_unique<Killer>(*object->get(), *this);
			break;
		}
		turn->setupChoices();
		//buildings

		turn->executeTurn();
	}
}

void Game::startRound()
{
	std::sort(characters.begin(), characters.end(), [](std::unique_ptr<CharacterCard> & a, std::unique_ptr<CharacterCard> & b) {return a->get_value() < b->get_value(); });
	for (auto &character : characters) {
		startTurn(character->get_name());
	}
	if (gameEnded)
	{
		endGame();
	}
	else cheatSetup();
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

bool Game::drawCards(const int amount, const int discard,const std::shared_ptr<ClientInfo> player)
{
	std::vector<std::unique_ptr<BuildingCard>> chosenbuildings;
	std::vector<std::unique_ptr<BuildingCard>> discardedbuildings;

	for (int i = 0; i < amount; i++)
	{
		if (buildings.front() != nullptr)
		{
			chosenbuildings.push_back(std::move(buildings.front()));
			buildings.erase(buildings.begin());
		}		
	}
	int j{ discard };
	while(j > 0 && !chosenbuildings.empty())
	{
		players[currentPlayer]->get_socket() << "kies een kaart om af te leggen.\r\n";
		int index = 0;
		for (auto &x : chosenbuildings)
		{
			if (x != nullptr) {
				players[currentPlayer]->get_socket() << *x << "\r\n";
			}
			index++;
		}
		players[currentPlayer]->get_socket() << machiavelli::prompt;
		int chosenIndex;
		bool done = false;
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = players[currentPlayer]->get_socket().readline([&chosenIndex, this](std::string input) {
					try { chosenIndex = std::stoi(input); }
					catch (...) { players[currentPlayer]->get_socket() << "kies een juiste waarde.\r\n" << machiavelli::prompt; }
				});
			}
			try
			{
				std::unique_ptr<BuildingCard> choice{ std::move(chosenbuildings.at(chosenIndex)) };
				discardedbuildings.push_back(std::move(choice));
				done = true;
			}
			catch (...)
			{
				players[currentPlayer]->get_socket() << "kies een juiste waarde.\r\n" << machiavelli::prompt;
			}
		}		
		j--;
	}
	
	for (auto &x : discardedbuildings) {
		x->set_discarded(true);
		buildings.push_back(std::move(x));
	}
	players[currentPlayer]->get_socket() << "De volgende kaarten zijn toegevoegd aan je hand:\r\n";
	for (auto &x : chosenbuildings) {
		if (x != nullptr) {
			players[currentPlayer]->get_socket() << *x << "\r\n";
			player->addCard(std::move(x));
		}
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

void Game::showCharacterChoices(const std::vector<std::string>& exceptions)
{
	for(auto &character : characters)
	{
		auto found = std::find(exceptions.begin(), exceptions.end(), character->get_name());
		if (!character->is_discarded() || found == exceptions.end())
		{
			players[currentPlayer]->get_socket() << *character << "\r\n";
		}		
	}
	players[currentPlayer]->get_socket() << machiavelli::prompt;
}

bool Game::characterExists(const std::string character)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_name() == character; }
	);
	if (object == characters.end()) return false;
	return true;
}

bool Game::canDestroyBuildings(const std::shared_ptr<ClientInfo> possiblePreacher)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator preacher =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_name() == "Prediker"; }
	);
	if (preacher != characters.end()) if (preacher->get()->get_player() == possiblePreacher) return false;
	return true;
	
}

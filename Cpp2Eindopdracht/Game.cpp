#include "Game.h"
#include <algorithm>
#include "Fileparser.h"
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
	endGame();
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
	for (auto &x : players) {
		drawCards(4, x);
		x->get_player().addGold(2);
	}
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
	currentPlayer = king;
	std::shuffle(buildings.begin(), buildings.end(), std::default_random_engine{});
	std::shuffle(characters.begin(), characters.end(), std::default_random_engine{});
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
		turn->addStandardChoices();
		turn->setupChoices();
		//buildings

		turn->executeTurn();
	}
}

void Game::startRound()
{
	for (auto &character : characters) {
		startTurn(character->get_name());
	}
	if (gameEnded)
	{
		endGame();
	}
}

void Game::endGame()
{
	int player1score = calculateScore(players[0]);
	int player2score = calculateScore(players[1]);

	if(player1score == player2score)
	{
		players[0]->get_socket() << "You have tied" << "\r\n";
		players[0]->get_socket() << "Your score was " << player1score << "\r\n";

		players[1]->get_socket() << "You have tied" << "\r\n";
		players[1]->get_socket() << "Your score was " << player2score << "\r\n";
	}
	else if(player1score > player2score)
	{
		players[0]->get_socket() << "You have won" << "\r\n";
		players[0]->get_socket() << "Your score was " << player1score << "\r\n";
		players[0]->get_socket() << "Your opponent's score was " << player2score << "\r\n";

		players[1]->get_socket() << "You have lost" << "\r\n";
		players[1]->get_socket() << "Your score was " << player2score << "\r\n";
		players[1]->get_socket() << "Your opponent's score was " << player1score << "\r\n";
	} else
	{
		players[0]->get_socket() << "You have lost" << "\r\n";
		players[0]->get_socket() << "Your score was " << player1score << "\r\n";
		players[0]->get_socket() << "Your opponent's score was " << player2score << "\r\n";

		players[1]->get_socket() << "You have won" << "\r\n";
		players[1]->get_socket() << "Your score was " << player2score << "\r\n";
		players[1]->get_socket() << "Your opponent's score was " << player1score << "\r\n";
	}

}

int Game::calculateScore(std::shared_ptr<ClientInfo> player)
{
	int score = 0;
	int numberofbuildings = 0;
	std::vector<std::string> colors;

	for (auto &building : player->get_buildings())
	{
		score += building->get_value();
		auto currentcolor = building->get_color();

		// if the current color is not in the vector
		if(std::find(colors.begin(), colors.end(), currentcolor) == colors.end())
		{
			colors.push_back(currentcolor);
		}
		numberofbuildings++;
	}

	if(colors.size() >= 5)
	{
		score += 3;
	}

	if (numberofbuildings >= 8)
	{
		// hier checken of ie t eerst was
		score += 2;
	}

	return score;
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

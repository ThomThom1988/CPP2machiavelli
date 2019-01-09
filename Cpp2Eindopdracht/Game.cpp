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
	endGame();
}

std::shared_ptr<ClientInfo> Game::getOtherPlayer(std::shared_ptr<ClientInfo> player) const
{
	ptrdiff_t pos = std::distance(players.begin(), std::find(players.begin(), players.end(), player));
	if (pos == 1) return players[0];
	return players[1];
}

void Game::changePlayer()
{
	players.at(currentPlayer)->get_socket().write("\r\nJe tegenstander is nu aan de beurt.\r\n");
	currentPlayer = currentPlayer == 0 ? 1 : 0;
}

void Game::startGame()
{
	std::shuffle(buildings.begin(), buildings.end(), std::default_random_engine{});
	for (auto &x : players) {
		
		auto other = getOtherPlayer(x);
		x->get_socket().write("\r\nJe speelt het spel tegen: " + other->get_player().get_name() + "\r\n\r\n");
		drawCards(4, 0, x);
		x->get_player().addGold(2);
		currentPlayer = currentPlayer == 0 ? 1 : 0;
	}
	king = players[0]->get_player().get_age() >= players[1]->get_player().get_age() ? 0 : 1;
	printKingInfo();
	currentPlayer = king;
	setState("start");
	players[currentPlayer]->get_socket() << "Wat wil je doen? : \r\n\r\n"
		<< "[cheatmodus] speler 1 is moordenaar en dief,speler 2 magier en koning, alle opties kunnen oneindig gebruikt worden.\r\n"
		<< "[kies karakters] beide spelers kiezen hun karakters.\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
	getOtherPlayer(players[currentPlayer])->get_socket() << "Wat wil je doen?(De koning start de keuzeronde) : \r\n\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
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
	if (inCheatMode()) cheatSetup();
	setState("setup started");
	currentRound++;
	std::shuffle(characters.begin(), characters.end(), std::default_random_engine{});
	discardCharacter(characters[0]->get_value());

	//karakter keuzes hier
	while (!allCharactersChosen())
	{
		players[currentPlayer]->get_socket().write("\r\nkies een karakter: \r\n");
		showCharacterChoices();
		int choice;
		bool done{ false };
		while (!done) {
			bool inputgotten{ false };
			while (!inputgotten) {
				inputgotten = players[currentPlayer]->get_socket().readline([&choice,this](std::string input) {
					try	{choice = std::stoi(input);	}
					catch (...){players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;}					
				});
			}
			done = chooseCharacter(choice, players[currentPlayer]);
			if(!done) players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
		}	

		if (!allCharactersChosen())
		{
			players[currentPlayer]->get_socket().write("\r\nkies een karakter om weg te leggen: \r\n");
			showCharacterChoices();
			done = false;
			while (!done) {
				bool inputgotten{ false };
				while (!inputgotten) {
					inputgotten = players[currentPlayer]->get_socket().readline([&choice, this](std::string input) {
						try { choice = std::stoi(input); }
						catch (...) { players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt; }
					});
				}
				done = discardCharacter(choice);
				if (!done) players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
			}
			changePlayer();
		}		
	}
	setState("setup");
	currentPlayer = king;
	players[currentPlayer]->get_socket() << "Wat wil je doen? : \r\n\r\n"
		<< "[start ronde] de ronde wordt gestart.\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
	getOtherPlayer(players[currentPlayer])->get_socket() << "Wat wil je doen?(De koning start de ronde) : \r\n\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
}

void Game::cheatSetup()
{
	for (auto &x : players) x->get_socket() << "\r\nCHEAT MODE ACTIVATED\r\n";
	activateCheatMode();
	setState("setup started");
	currentRound++;
	chooseCharacter(1, players.at(0));
	chooseCharacter(2, players.at(0));
	chooseCharacter(3, players.at(1));
	chooseCharacter(4, players.at(1));
	discardCharacter(5);
	discardCharacter(6);
	discardCharacter(7);
	discardCharacter(8);
	setState("setup");
	players[currentPlayer]->get_socket() << "Wat wil je doen? : \r\n\r\n"
		<< "[start ronde] de ronde wordt gestart.\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
	getOtherPlayer(players[currentPlayer])->get_socket() << "Wat wil je doen?(De koning start de ronde) : \r\n\r\n"
		<< "[help] spelinformatie.\r\n"
		<< "[quit] stap uit het spel.\r\n"
		<< "[quit_server] stop de server.\r\n"
		<< machiavelli::prompt;
}

void Game::startTurn(std::string character)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_name() == character; }
	);
	if (object->get()->get_player() == nullptr || character == killedCharacter)
	{
		for (auto &x : players) x->get_socket() << "\r\nDe " << character << " komt deze ronde niet aan de beurt.\r\n";
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

			dief->get()->get_player()->get_socket() << "\r\nJe hebt " << std::to_string(amount) <<" goudstukken gestolen van de " << character << "!\r\n";
			object->get()->get_player()->get_socket() << "\r\nAl jouw goudstukken zijn gestolen door de dief\r\n";
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
		turn->addPurpleChoices();
		turn->executeTurn();
	}
}

void Game::startRound()
{
	setState("round started");
	getOtherPlayer(players[currentPlayer])->get_socket() << "\r\nJe tegenstander is de ronde begonnen\r\n";
	std::sort(characters.begin(), characters.end(), [](std::unique_ptr<CharacterCard> & a, std::unique_ptr<CharacterCard> & b) {return a->get_value() < b->get_value(); });
	for (auto &character : characters) {
		startTurn(character->get_name());
	}
	if (gameEnded)
	{
		for (auto &x : players) x->get_socket() << "\r\nHet spel is afgelopen, de scores worden nu uitgerekend.\r\n";
		endGame();
	}
	else {
		for (auto &x : players) x->get_socket() << "\r\nHet spel is afgelopen, de koning start de nieuwe ronde op.\r\n";
		setState("start");
		currentPlayer = king;
		players[currentPlayer]->get_socket() << "Wat wil je doen? : \r\n\r\n"
			<< "[kies karakters] beide spelers kiezen hun karakters voor de volgende ronde.\r\n"
			<< "[help] spelinformatie.\r\n"
			<< "[quit] stap uit het spel.\r\n"
			<< "[quit_server] stop de server.\r\n"
			<< machiavelli::prompt;
		getOtherPlayer(players[currentPlayer])->get_socket() << "Wat wil je doen?(De koning start de keuzeronde) : \r\n\r\n"
			<< "[help] spelinformatie.\r\n"
			<< "[quit] stap uit het spel.\r\n"
			<< "[quit_server] stop de server.\r\n"
			<< machiavelli::prompt;
	}
}

void Game::endGame()
{
	int player1score = calculateScore(players[0]);
	int player2score = calculateScore(players[1]);

	if(player1score == player2score)
	{
		players[0]->get_socket() << "\r\nJe hebt gelijkgespeeld" << "\r\n";
		players[0]->get_socket() << "De score was " << player1score << "\r\n";

		players[1]->get_socket() << "\r\nJe hebt gelijkgespeeld" << "\r\n";
		players[1]->get_socket() << "De score was " << player2score << "\r\n";
	}
	else if(player1score > player2score)
	{
		players[0]->get_socket() << "\r\nJe hebt gewonnen" << "\r\n";
		players[0]->get_socket() << "De score was " << player1score << "\r\n";
		players[0]->get_socket() << "De score van de tegenstander was " << player2score << "\r\n";

		players[1]->get_socket() << "\r\nJe hebt verloren" << "\r\n";
		players[1]->get_socket() << "De score was " << player2score << "\r\n";
		players[1]->get_socket() << "De score van de tegenstander was " << player1score << "\r\n";
	} else
	{
		players[0]->get_socket() << "\r\nJe hebt verloren" << "\r\n";
		players[0]->get_socket() << "De score was " << player1score << "\r\n";
		players[0]->get_socket() << "De score van de tegenstander was " << player2score << "\r\n";

		players[1]->get_socket() << "\r\nou have won" << "\r\n";
		players[1]->get_socket() << "De score was " << player2score << "\r\n";
		players[1]->get_socket() << "De score van de tegenstander was " << player1score << "\r\n";
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
		//purple cards

		if (building->get_name() == "Drakenpoort" || building->get_name() == "Universiteit")score += 8;
		if (building->get_name() == "Hof der Wonderen")
		{
			std::string color;
			player->get_socket() << "\r\nKies een kleur die het hof aanneemt";
			bool colorchoice{ false };
			while (!colorchoice)
			{
				bool inputgotten{ false };
				while (!inputgotten) {
					inputgotten = player->get_socket().readline([&color](std::string input) {
						color = input;
					});
				}
				if (color == "groen" || color == "geel" || color == "blauw" || color == "rood" || color == "lila") colorchoice = true;
				else player->get_socket() << "\r\nKies uit groen/geel/blauw/rood/lila";
			}
			building->set_color(color);
		}

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
		if (player->firstToGet8)
			score += 4;
		else	
			score += 2;
	}

	return score;
}

void Game::printKingInfo()
{
	auto kingsname = players.at(king)->get_player().get_name();
	players.at(king)->get_socket().write("\r\nJe bent de koning.\r\n\r\n");
	auto other = getOtherPlayer(players.at(king));
	other->get_socket().write("\r\n" + kingsname + " is de koning.\r\n\r\n");
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
		players[currentPlayer]->get_socket() << "\r\nkies een kaart om af te leggen.\r\n";
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
					catch (...) { players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt; }
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
				players[currentPlayer]->get_socket() << "\r\nkies een juiste waarde.\r\n" << machiavelli::prompt;
			}
		}		
		j--;
	}
	
	for (auto &x : discardedbuildings) {
		x->set_discarded(true);
		buildings.push_back(std::move(x));
	}
	players[currentPlayer]->get_socket() << "\r\nDe volgende kaarten zijn toegevoegd aan je hand:\r\n";
	for (auto &x : chosenbuildings) {
		if (x != nullptr) {
			players[currentPlayer]->get_socket() << *x << "\r\n";
			player->addCard(std::move(x));
		}
	}
	players[currentPlayer]->get_socket() << "\r\n";
	return true;
}

bool Game::chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player)
{
	std::vector<std::unique_ptr<CharacterCard>>::iterator object =
		std::find_if(characters.begin(), characters.end(),
			[&](std::unique_ptr<CharacterCard> & obj) { return obj->get_value() == character; }
	);
	if (object->get() == nullptr) return false;
	if (object->get()->is_discarded()) return false;
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
	if (object->get() == nullptr) return false;
	if (object->get()->is_discarded()) return false;
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
		if (found == exceptions.end())
		{
			players[currentPlayer]->get_socket() << *character << "\r\n";
		}	
	}
	players[currentPlayer]->get_socket() << machiavelli::prompt;
}

void Game::showCharacterChoices()
{
	for (auto &character : characters)
	{
		if (!character->is_discarded())
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

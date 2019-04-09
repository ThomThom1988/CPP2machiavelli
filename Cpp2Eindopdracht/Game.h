#pragma once
#include <vector>
#include "Socket.h"
#include "ClientInfo.h"
#include "Fileparser.h"
#include "BuildingCard.h"
#include "CharacterCard.h"
#include <random>
#include <chrono> 

//stond zo in voorbeeldcode
namespace machiavelli {
	const int tcp_port{ 1080 };
	const std::string prompt{ "machiavelli> " };
}

//class CardProperty;

class Game
{
public:
	Game() : currentRound{ 0 }, gameEnded{ false }, cheatmode{ false }, focus{ false }	{
		Fileparser parser;
		characters = parser.readCharacterCards();
		buildings = parser.readBuildingCards();
		engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
	}
	~Game();
	void addPlayer(const std::shared_ptr<ClientInfo> player) { players.push_back(player); }
	int amountOfPlayers() const { return players.size(); }
	std::shared_ptr<ClientInfo> getCurrentPlayer() const { return players.at(currentPlayer); }
	std::shared_ptr<ClientInfo> getOtherPlayer(std::shared_ptr<ClientInfo>) const;
	std::string getState() const { return state; }
	void setState(const std::string currentState) { state = currentState; }
	void changePlayer();
	void startGame();
	bool allCharactersChosen() const;
	void roundSetup();
	void cheatSetup();
	void startTurn(std::string character);
	void startRound();
	void endGame();
	void gameEndsAfterRound() { gameEnded = true; }
	int calculateScore(const std::shared_ptr<ClientInfo> player);
	void printKingInfo() const;
	bool drawCards(const int amount, const int discard, const std::shared_ptr<ClientInfo> player);
	bool chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player);
	bool discardCharacter(const int disposedCharacter);
	void resetCharacters();
	void addBuildingToDraw(std::unique_ptr<BuildingCard> card) { buildings.push_back(std::move(card)); }
	void addCharacterToDraw(std::unique_ptr<CharacterCard> card) { characters.push_back(std::move(card)); }
	void showCharacterChoices(const std::vector<std::string>& vec) const;
	void showCharacterChoices() const;
	bool characterExists(const std::string character);
	void murderCharacter(const std::string character) { killedCharacter = character; }
	void stealFromCharacter(const std::string character) { goldStolenCharacter = character; }
	bool canDestroyBuildings(const std::shared_ptr<ClientInfo> possiblePreacher);
	
	std::string getMurderedCharacter() const { return killedCharacter; }
	bool inCheatMode() const { return cheatmode; }
	void activateCheatMode() { cheatmode = true; }
	bool gameHasFocus() const { return focus; }
	

private:
	std::vector<std::shared_ptr<ClientInfo>> players;
	std::vector<std::unique_ptr<BuildingCard>> buildings;
	std::vector<std::unique_ptr<CharacterCard>> characters;
	int currentPlayer;
	int currentRound;
	int king;
	std::string killedCharacter;
	std::string goldStolenCharacter;
	std::string state;
	bool gameEnded;
	bool cheatmode;
	bool focus;
	std::default_random_engine engine;
};


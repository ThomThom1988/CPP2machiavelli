#pragma once
#include <vector>
#include "Socket.h"
#include "ClientInfo.h"
#include "Fileparser.h"
#include "BuildingCard.h"
#include "CharacterCard.h"

namespace machiavelli {
	const int tcp_port{ 1080 };
	const std::string prompt{ "machiavelli> " };
}

//class CardProperty;

class Game
{
public:
	Game() : currentRound{ 0 }, gameEnded{ false }, gameRunning{false}	{
		Fileparser parser;
		characters = parser.readCharacterCards();
		buildings = parser.readBuildingCards(); 
	}
	~Game();
	void addPlayer(const std::shared_ptr<ClientInfo> player) { players.push_back(player); }
	int amountOfPlayers() const { return players.size(); }
	std::shared_ptr<ClientInfo> getCurrentPlayer() const { return players.at(currentPlayer); }
	std::shared_ptr<ClientInfo> getOtherPlayer(std::shared_ptr<ClientInfo>) const;
	void changePlayer();
	void startGame();
	bool allCharactersChosen();
	void roundSetup();
	void cheatSetup();
	void startTurn(std::string character);
	void startRound();
	void endGame();
	void gameEndsAfterRound() { gameEnded = true; }
	int calculateScore(std::shared_ptr<ClientInfo> player);
	void printKingInfo();
	bool drawCards(const int amount, const int discard, const std::shared_ptr<ClientInfo> player);
	bool chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player);
	bool discardCharacter(const int disposedCharacter);
	bool getGameRunning() const { return gameRunning; }
	void resetCharacters();
	void addBuildingToDraw(std::unique_ptr<BuildingCard> card) { buildings.push_back(std::move(card)); }
	void addCharacterToDraw(std::unique_ptr<CharacterCard> card) { characters.push_back(std::move(card)); }
	void showCharacterChoices(const std::vector<std::string>& vec = std::vector<std::string>());
	bool characterExists(const std::string character);
	void murderCharacter(const std::string character) { killedCharacter = character; }
	void stealFromCharacter(const std::string character) { goldStolenCharacter = character; }
	bool canDestroyBuildings(const std::shared_ptr<ClientInfo> possiblePreacher);
	std::string getMurderedCharacter() const { return killedCharacter; }
	

private:
	std::vector<std::shared_ptr<ClientInfo>> players;
	std::vector<std::unique_ptr<BuildingCard>> buildings;
	std::vector<std::unique_ptr<CharacterCard>> characters;
	int currentPlayer;
	int currentRound;
	int king;
	std::string killedCharacter;
	std::string goldStolenCharacter;
	bool gameEnded;
	bool gameRunning;
};


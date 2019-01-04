#pragma once
#include <vector>
#include "Socket.h"
#include "ClientInfo.h"
#include "Fileparser.h"

class Game
{
public:
	Game() : currentRound{0}, gameEnded{ false }
	{Fileparser parser;
		drawCharacters = parser.readCharacterCards();
	drawBuildings = parser.readBuildingCards(); }
	~Game();
	void addPlayer(const std::shared_ptr<ClientInfo> player) { players.push_back(player); }
	int amountOfPlayers() { return players.size(); }
	std::shared_ptr<ClientInfo> getCurrentPlayer() { return players.at(currentPlayer); }
	std::shared_ptr<ClientInfo> getOtherPlayer(const std::shared_ptr<ClientInfo>);
	void changePlayer();
	void startGame();
	void roundSetup();
	void cheatSetup();
	void printKingInfo();
	bool drawCards(const int amount, const std::shared_ptr<ClientInfo> player);
	bool chooseCharacter(const int character, const std::shared_ptr<ClientInfo> player);
	bool discardCharacter(const int disposedCharacter);
	void addBuildingToDraw(std::unique_ptr<Card> card) { drawBuildings.push_back(std::move(card)); }
	void addCharacterToDraw(std::unique_ptr<Card> card) { drawCharacters.push_back(std::move(card)); }
	void addBuildingToDiscard(std::unique_ptr<Card> card) { discardedBuildings.push_back(std::move(card)); }
	void addCharacterToDiscard(std::unique_ptr<Card> card) { discardedCharacters.push_back(std::move(card)); }

private:
	std::vector<std::shared_ptr<ClientInfo>> players;
	std::vector<std::unique_ptr<Card>> drawBuildings;
	std::vector<std::unique_ptr<Card>> discardedBuildings;
	std::vector<std::unique_ptr<Card>> drawCharacters;
	std::vector<std::unique_ptr<Card>> discardedCharacters;
	int currentPlayer;
	int currentRound;
	int king;
	bool gameEnded;
};


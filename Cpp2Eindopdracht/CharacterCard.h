#pragma once
#include "Card.h"
#include "Player.h"
#include "ClientInfo.h"

class CharacterCard :
	public Card
{
public:
	CharacterCard();
	~CharacterCard();

	const std::shared_ptr<ClientInfo>& get_player() const { return player; }
	void set_Player(const std::shared_ptr<ClientInfo> playerInThisRound) { player = playerInThisRound; }
	void reset_player() { player = nullptr; }
	friend std::istream & operator>>(std::istream & is, CharacterCard & c);
	friend const Socket & operator<<(Socket & s, CharacterCard & c);

private:
	std::shared_ptr<ClientInfo> player;
};


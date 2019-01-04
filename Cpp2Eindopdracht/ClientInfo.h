//
// ClientInfo.h
//
// Created by Jeroen de Haas on 22/11/2016.
// Copyright (c) 2016 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#ifndef _CLIENT_INFO_H
#define _CLIENT_INFO_H

#include <string>
#include <utility>

#include "Socket.h"
#include "Player.h"

class ClientInfo {
    Socket _socket;
    Player _player;
	std::vector<std::unique_ptr<Card>> hand;
	std::vector<std::unique_ptr<Card>> buildings;
public:
    ClientInfo(Socket socket, Player player)
        : _socket{std::move(socket)}, _player{player}{}
    Socket& get_socket() { return _socket; }
    const Socket& get_socket() const { return _socket; }
    Player& get_player() { return _player; }
    const Player& get_player() const { return _player; }
	//void addCard(const std::unique_ptr<Card> card) { hand.push_back(std::move(card)); }
	//void addBuilding(const std::unique_ptr<Card> card) { buildings.push_back(std::move(card)); }
};


#endif

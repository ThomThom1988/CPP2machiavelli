//
//  Player.hpp
//  socketexample
//
//  Created by Bob Polis on 23-11-15.
//  Copyright © 2015 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//

#pragma once
#include "Card.h"
#include <string>
#include <vector>

class Player {
public:
	Player() {}
	Player(const std::string& name, const int& age) : name{ name }, age{ age }, goldPieces{ 0 }{}

	std::string get_name() const { return name; }
	int get_age() const { return age; }
	void set_name(const std::string& new_name) { name = new_name; }
	void addGold(const int amount) { goldPieces += amount; }
	int get_gold() const { return goldPieces; }

private:
	std::string name;
	int age;
	int goldPieces;
};

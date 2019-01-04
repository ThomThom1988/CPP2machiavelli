#pragma once
#include <string>
#include <vector>
#include "Card.h"

class NewPlayer
{
public:
	NewPlayer();
	NewPlayer(const std::string& name, const int& age) : name{ name }, age{ age }, goldPieces{ 2 }{}
	~NewPlayer();

	std::string get_name() const { return name; }
	int get_age() const { return age; }
	void set_name(const std::string& new_name) { name = new_name; }

private:
	std::string name;
	int age;
	int goldPieces;
	std::vector<std::unique_ptr<Card>> hand;
	std::vector<Card> buildings;
};


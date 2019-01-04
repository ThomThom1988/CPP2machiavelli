#pragma once
#include <string>
#include "CardProperty.h"

class Card
{
public:
	Card();
	const std::string& get_name() const { return name; }
	const int& get_value() const { return value; }
	void set_value(const int cardValue) { value = cardValue; }
	void set_name(const std::string cardName) { name = cardName; }
	virtual ~Card();
protected:
	std::string name;
	CardProperty property;
	int value;
};


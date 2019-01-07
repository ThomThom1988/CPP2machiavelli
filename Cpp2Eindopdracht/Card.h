#pragma once
#include <string>
#include "CardProperty.h"

class Card
{
public:
	Card();
	const std::string& get_name() const { return name; }
	const int& get_value() const { return value; }
	const bool& is_discarded() const { return discarded; }
	void set_value(const int cardValue) { value = cardValue; }
	void set_name(const std::string cardName) { name = cardName; }
	void set_discarded(const bool value) { discarded = value; }
	virtual ~Card();
protected:
	std::string name;
	int value;
	bool discarded;
};


#pragma once
#include "Card.h"
#include <vector>

class BuildingCard :
	public Card
{
public:
	BuildingCard();
	~BuildingCard();
	static std::vector<std::string> split(const std::string& stringToBeSplitted, std::string delimeter);
	const std::string& get_color() const { return color; }
	const std::string& get_description() const { return description; }
	void set_color(const std::string cardColor) { color = cardColor; }
	void set_description(const std::string cardDescription) { description = cardDescription; }
	friend std::istream & operator>>(std::istream & is, BuildingCard & c);

private:
	std::string color;
	std::string description;
};


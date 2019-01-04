#include "BuildingCard.h"
#include <sstream>
#include <vector>
#include <iostream>


BuildingCard::BuildingCard()
{
}


BuildingCard::~BuildingCard()
{
}

std::vector<std::string> BuildingCard::split(const std::string & stringToBeSplitted, std::string delimeter)
{
	std::vector<std::string> splittedString;
	int startIndex = 0;
	int  endIndex = 0;
	while ((endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size())
	{
		std::string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
		splittedString.push_back(val);
		startIndex = endIndex + delimeter.size();
	}
	if (startIndex < stringToBeSplitted.size())
	{
		std::string val = stringToBeSplitted.substr(startIndex);
		splittedString.push_back(val);
	}
	return splittedString;
}

std::istream & operator>>(std::istream & is, BuildingCard & c)
{
	std::string s;
	std::getline(is, s);
	auto variables = BuildingCard::split(s, ";");
	if (variables.size() == 3) variables.push_back("");
	try
	{
		c.set_name(variables.at(0));
		c.set_value(stoi(variables.at(1)));
		c.set_color(variables.at(2));
		c.set_description(variables.at(3));
	}
	catch (...)
	{
		std::cerr << "Er is een fout opgetreden bij het laden van de gebouwen" << '\n';
	}	
	return is;
}

std::ostream & operator<<(std::ostream & os, BuildingCard & c)
{
	if (c.get_description() != "") return os << c.get_name() << "(" << c.get_value() << ", " << c.get_color() << ", " << c.get_description() << ")";
	return os << c.get_name() << "(" << c.get_value() << ", " << c.get_color() << ")";
}

#include "CharacterCard.h"



CharacterCard::CharacterCard()
{
}


CharacterCard::~CharacterCard()
{
}

std::istream & operator>>(std::istream & is, CharacterCard & c)
{
	int value{};
	std::string name{};
	char ch;
	if (is >> value >> ch >> name)
	{
		c.set_value(value);
		c.set_name(name);
	}
	return is;
}

const Socket & operator<<(Socket & s, CharacterCard & c)
{
	return s << "[" << c.get_value() << "] " << c.get_name();
}

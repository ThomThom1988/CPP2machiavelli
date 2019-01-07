#include "CardProperty.h"

CardProperty::~CardProperty()
{
}

std::ostream & operator<<(std::ostream & os, CardProperty & p)
{
	return os << p.get_description();
}

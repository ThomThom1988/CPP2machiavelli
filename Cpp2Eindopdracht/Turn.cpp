#include "Turn.h"
#include "Properties/Murder.h"
#include "Properties/Thief.h"
#include "Properties/SwitchWithOther.h"
#include "Properties/SwitchHand.h"
#include "Properties/ColorBonus.h"
#include "Properties/DrawCards.h"
#include "Properties/Destroy.h"
#include "Properties/DrawCoins.h"

Turn::Turn(const CharacterCard& characterCard) : character{characterCard}
{
	properties.push_back(std::make_unique<DrawCards>(2));
	properties.push_back(std::make_unique<DrawCoins>(2));
	switch (characterCard.get_value())
	{
	case 1:
		properties.push_back(std::make_unique<Murder>());
		break;
	case 2:
		//properties.push_back(std::make_unique<Thief>());
		break;
	case 3:
		properties.push_back(std::make_unique<SwitchHand>());
		properties.push_back(std::make_unique<SwitchWithOther>());
		break;
	case 4:
		properties.push_back(std::make_unique<ColorBonus>("yellow"));
		break;
	case 5:
		properties.push_back(std::make_unique<ColorBonus>("blue"));
		break;
	case 6:
		properties.push_back(std::make_unique<DrawCoins>(1));
		properties.push_back(std::make_unique<ColorBonus>("green"));
		break;
	case 7:
		properties.push_back(std::make_unique<DrawCards>(2));
		break;
	case 8:
		properties.push_back(std::make_unique<ColorBonus>("red"));
		properties.push_back(std::make_unique<Destroy>());
		break;
	}
}

Turn::~Turn()
{
}

void Turn::chooseOption(const int option)
{
	properties[option]->useProperty();
	//delete option
	//evt delete andere option
}

std::ostream & operator<<(std::ostream & os, Turn & t)
{
	std::string result;
	for (int i = 0; i < t.properties.size();i++)
	{
		result += "[";
		result += i;
		result += "] ";
		result += t.properties[i]->get_description();
		result += ".\r\n";
	}
	result += "[klaar] Beëindig beurt.\r\n";
	return os << result;
}

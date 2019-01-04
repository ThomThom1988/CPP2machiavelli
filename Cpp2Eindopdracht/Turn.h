#pragma once
#include "CharacterCard.h"

class Turn
{
public:
	Turn(const CharacterCard& characterCard) : character{characterCard}{}
	~Turn();

private:
	CharacterCard character;
};


#pragma once
#include <string>

class CardProperty
{
public:
	CardProperty() {};
	virtual ~CardProperty();
	friend std::ostream & operator<<(std::ostream & os, CardProperty & p);
	std::string get_description() { return description; }
	//mss template?? (typename t Args args)
	virtual bool useProperty() = 0;
private:	
	bool used;
protected:
	std::string description;	
};


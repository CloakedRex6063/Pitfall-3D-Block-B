#include "randomName.h"
#include <cstdlib>
#include <ctime>
#include "iterator"

char RandomName::GetRandomVowel() const
{
	return vowels[rand() % std::size(vowels)];
}

char RandomName::GetRandomConsonant() const
{
	return consonants[rand() % std::size(consonants)];
}

std::string RandomName::GenerateRandomName(int length) const
{
	std::string name;

	for (int i = 0; i < length; ++i) 
	{
		if (i % 2 == 0) 
		{
			name += GetRandomConsonant();
		}
		else 
		{
			name += GetRandomVowel();
		}
	}
	name[0] = static_cast<char>(std::toupper(name[0]));

	return name;
}

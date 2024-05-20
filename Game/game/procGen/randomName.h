#pragma once
#include <string>

class RandomName
{
private:
	const char vowels[5] = {'a', 'e', 'i', 'o', 'u'};
	const char consonants[21] = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};

	char GetRandomVowel() const;

	char GetRandomConsonant() const;

public:
	std::string GenerateRandomName(int length) const;
};

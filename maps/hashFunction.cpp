#include "include/HashTable.h"

unsigned int hash_function_s(string s, unsigned int tableSize)
{
	unsigned int h = 47281232;
	for(int i = 0; i < s.length(); i++)
	{
		h = (h + s[i]) * 9939;
		h %= tableSize;
	}
	return h;
}

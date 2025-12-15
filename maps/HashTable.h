#ifndef HASHTABLE
#define HASHTABLE

#include <string>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

unsigned int hash_function_s(string s, unsigned int tableSize);


template<typename K, typename V >
class Hashtable
{
	protected :
	template<typename T>
	struct HashLink
	{
		T data;
		HashLink* next;
		HashLink(T d): data(d) {next = NULL;};
		HashLink(T d, HashLink* n): data(d), next(n){};
		//HashLink(T d, HashLink* n): data(d), next(n){};
		//int compareTo(HashNode<k,v>
	};
	HashLink<V>** table;
	int numBuckets, numEntries;
	void resize(std::function<unsigned int(K, unsigned int)>);
	K getKeyFromData(V*);
	K getKeyFromData(V&);

	public : 	

	Hashtable(); 
	Hashtable(int);
	~Hashtable();
	V* getData(V, std::function<unsigned int(K, unsigned int)>);
	std::vector<V> getData(K, std::function<unsigned int(K, unsigned int)>);
	void add(V, std::function<unsigned int(K, unsigned int)>);
	int size();
};


template<typename K, typename V>
K Hashtable<K,V>::getKeyFromData(V* ptr) { return ptr->getKey(); }

template<typename K, typename V>
Hashtable<K,V>::~Hashtable()
{
	for(int i = 0; i < numBuckets; i++)
	{
		HashLink<V>* current = table[i];
		while(current)
		{
			HashLink<V>* tmp = current;
			current = current->next;
			delete tmp;
		}
	}
	delete[] table;
}

template<typename K, typename V>
Hashtable<K,V>::Hashtable()
{
	numBuckets = 71;
	numEntries = 0;
	table = new HashLink<V>*[numBuckets]; // array of pointers
	for(int i = 0; i < numBuckets; i++)
	{
		table[i] = NULL;
	}
}

template<typename K, typename V>
Hashtable<K,V>::Hashtable(int size)
{
	numBuckets = size;
	numEntries = 0;
	table = new HashLink<V>*[numBuckets];
	for(int i = 0; i < numBuckets; i++)
	{
		table[i] = NULL;
	}
}

template<typename K, typename V>
void Hashtable<K,V>::add(V val, std::function<unsigned int(K, unsigned int)> hashFunction)
{
	K keyV = val->getKey();
	unsigned int key = hashFunction(keyV, numBuckets); // create key
	numEntries++;
	if(numEntries > numBuckets) // check resize
	{
		resize(hashFunction);
		key = hashFunction(keyV, numBuckets); // rehash new item with updated size
	}
	table[key] = new HashLink<V>(val, table[key]); // add to head
}

template<typename K, typename V>
void Hashtable<K,V>::resize(std::function<unsigned int(K, unsigned int)> hashFunction)
{
	int oldSize = numBuckets;
	numBuckets = numBuckets*2;
	HashLink<V>** newTable = new HashLink<V>*[numBuckets];
	
	for(int i = 0; i < numBuckets; i++) // make each buket empty
	{
		newTable[i] = NULL;
	}
	for(int i = 0; i < oldSize; i++)
	{
		HashLink<V>* link = table[i];	
		while(link) // rehash links in chain to new table
		{
			K keyV = link->data->getKey();
			unsigned int newHash = hashFunction(keyV, numBuckets);
			HashLink<V>* next = link->next;
			// insert at head of new bucket
			link->next = newTable[newHash];
			newTable[newHash] = link;
			link  = next; 
		}
	}
	delete[] table;
	table = newTable;
}


// returns data based on the data's key, keyV. Iterates through entire bucket with iterator index.
template<typename K, typename V>
std::vector<V> Hashtable<K,V>::getData(K keyV, std::function<unsigned int(K, unsigned int)> hashFunction)
{
	std::vector<V> bucket = std::vector<V>();
	unsigned int key = hashFunction(keyV, numBuckets);	
	HashLink<V>* current = table[key];

	while(current != NULL) // index acts as iterator through the bucket for matching key data values
	{
		bucket.push_back(current->data);
		current = current->next;
	}
	return bucket;
}

template<typename K, typename V>
int Hashtable<K,V>::size() { return numBuckets; }

#endif

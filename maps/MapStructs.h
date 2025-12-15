#ifndef MAP_STRUCTS_H
#define MAP_STRUCTS_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Intersection;

struct Place 
{
	int code, population, roadCode;
	string stateAbbrev, name;
	float area, latitude, longitude, distanceFrom;

	// constructors initialization
	Place() : code(0), population(0), roadCode(0),
		stateAbbrev(""), name(""),
		area(0.0), latitude(0.0), longitude(0.0), distanceFrom(0.0){};

	Place(int code, string abbrev, string name, int pop, float area,
		float lat, float lon, int roadC, float dFrom) : 
		code(code), stateAbbrev(abbrev), name(name), population(pop),
		area(area), latitude(lat), longitude(lon), distanceFrom(dFrom){};
	Place(const Place &p) : code(p.code), population(p.population), 
		roadCode(p.roadCode),stateAbbrev(p.stateAbbrev), name(p.name),
	       	area(p.area), latitude(p.latitude), longitude(p.longitude), distanceFrom(p.distanceFrom){};
	
	// operator overloading
	bool operator==(const Place& rhs)
	{
		return (rhs.name == name && rhs.stateAbbrev == stateAbbrev);
	}

	friend std::ostream& operator<<(std::ostream& out, const Place& p)
	{
		out << "Place is " << p.name << ", " << p.stateAbbrev << ", pop " << p.population;
		cout << "(" << p.longitude << ", " << p.latitude << "), closest intersection " << p.roadCode;
		return out;
	}
	// methods
	std::string getKey()
	{
		return name;
	}
};

struct State
{
	std::string abbrev, name;
	State() : abbrev(""), name(""){};
	// operator overloading
	bool operator==(const State& rhs)
	{
		return (rhs.abbrev == abbrev);
	}
	//methods
	std::string getKey()
	{
		return abbrev;
	}
};

struct Road
{
	string name, type;
	Intersection* intA;
	Intersection* intB;
	float length;

	Road(): name(""),type(""), intA(NULL), intB(NULL), length(0.0){};

	friend ostream& operator<<(ostream& out, const Road& r)
	{
		out << r.name << " | " << r.length << " miles ";
		return out;
	}
};

struct Intersection
{
	int num;
	float latitude, longitude, distanceToPlace;
	string stateAbbrev, name;	
	vector<Road*> exits;
	bool visited;
	
	Intersection(): num(0), latitude(0.0), longitude(0.0), distanceToPlace(0.0), 
			stateAbbrev(""), name(""), exits(0,0), visited(false) {};

	int getKey(){ return num;}

	friend ostream& operator<<(ostream& out, const Intersection& i)
	{
		out << i.name << "\n--------------------\n";
		out << "Intersection # " << i.num <<  " in " << i.stateAbbrev;
		out << " Longitude: " << i.longitude << " Latitude: " << i.latitude << endl;
		out << "Exits : ";
		for(int index=0;index<i.exits.size();index++) out << i.exits[index]->name << " ";
		return out;
	}
};

#endif

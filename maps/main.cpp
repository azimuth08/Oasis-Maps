#include <iostream>
#include <fstream>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "HashTable.h"
#include "readFiles.h"
#include "draw.h"
#include "PriorityQueue.h"

using namespace std; 

// get userr's starting point in map
bool prompt(Hashtable<string, Place*> &pT, vector<Intersection*> &intersList, Hashtable<string, State*> &sT, Intersection *&closestInters)
{
	vector<Place*> placeBucket;
	string line, placeN, userState;
	int key  = -1;
	
	getline(cin, line);

	stringstream userInput(line);
	while(userInput >> userState){};

	if(line.size() < 2 || !is_place(pT,line))
	{
		cout << "Invalid response\n";
		return false;
	}
	if(!is_state(sT,userState))
	{
        	userState = get_state_abbrev(pT,sT,line);
	}

	placeN = line;
	placeBucket = pT.getData(placeN, hash_function_s);

	if(placeBucket.size() == 0)
	{
		cout << "Could not find place, try again\n";
		return false;
	}
	for(int i=0;i<placeBucket.size();i++)
	{
		if(placeBucket[i] == NULL) continue;
		if((placeBucket[i]->name == placeN) && (placeBucket[i]->stateAbbrev == userState))
		{
			cout << *placeBucket[i] << endl << endl;
			key = placeBucket[i]->roadCode;
			break;
		}
		if(i == placeBucket.size() - 1)
		{
			cout << "Could not find place, try again\n";
			return false;
		}
	}

	if(key == -1)
	{
		cout << "Coutld not find place, try again\n";
		return false;
	}
	
	closestInters = intersList[key];
	return true;
}

// finds the degrees between two coordinates and divides by 45 to get the direction
string find_direction(Intersection* beginning, Intersection* end)
{
	string directions[8] = {"N", "Ne", "E", "Se", "S", "Sw", "W", "Nw"};
	float diffLat = beginning->latitude - end->latitude ;
	float diffLon = end->longitude - beginning->longitude;
	
	float degree = atan2(diffLat, diffLon)*180.0/M_PI;
	degree = 90 - degree; // adjust so 0 deg == N
	if(degree < 0) degree += 360.0;
	int coordIndex = (int)(degree / 45) % 8; // #TODO 
	
	return directions[coordIndex];
}
/*
string find_direction(Intersection* end, string placeName)
{
	string directions[8] = {"N", "Ne", "E", "Se", "S", "Sw", "W", "Nw"};
	float diffLat = beginning->latitude - end->latitude ;
	float diffLon = end->longitude - beginning->longitude;
	
	float degree = atan2(diffLat, diffLon)*180.0/M_PI;
	degree = 90 - degree; // adjust so 0 deg == N
	if(degree < 0) degree += 360.0;
	int coordIndex = (int)(degree / 45) % 8; // #TODO 
	
	return directions[coordIndex];
}
*/
void resetVisited(vector<Intersection*>& intersList)
{
	for(int i=0;i<intersList.size();i++)
	{
		intersList[i]->visited = false;
	}
}

void printMinPath(PathNode* minPath)
{
	vector<PathNode*> path;
	PathNode* curPath = minPath;
	int totalW = minPath->weight;

	while(curPath != NULL)
	{
		path.push_back(curPath);
		curPath = curPath->parent;
	}
	// print start
	PathNode* start = path.back();
	path.pop_back();
	Intersection* startInters = start->intersectionP;
	cout << "Start at " << startInters->distanceToPlace << " miles from " << startInters->name << " " << startInters->stateAbbrev << endl;
	// print road leading from start to next intersection
	if(path.size() > 0)
	{
		PathNode* nextNode = path.back();
		Intersection* nextInters = nextNode->intersectionP;
		Road* startRoad;
		for(int i=0;i<startInters->exits.size();i++) // find the right road from start because its path road is initialized to NULL
		{
			if((startInters->exits[i]->intA->name == startInters->name || startInters->exits[i]->intB->name == startInters->name) &&
				(startInters->exits[i]->intA->name == nextInters->name || startInters->exits[i]->intB->name == nextInters->name))
			{
				startRoad = startInters->exits[i];
			}
		}
 
		string direction = find_direction(nextInters, startInters);
    
		cout << "then take " << startRoad->name << " " 
			 << startRoad->length << " miles " << direction 
			 << " to " << nextInters->name << " " 
			 << nextInters->stateAbbrev << endl;
	}
	// loop printing rest of the path 
	while(path.size() > 0)
	{
		start = path.back();
		path.pop_back();
		startInters = start->intersectionP;
		Road* curRoad = start->roadP;
		if(path.size() == 0) break;
		// find direction from cur to next stop
		PathNode* next = path.back();
		Intersection* nextInters = next->intersectionP;

		string direction = find_direction(nextInters, startInters);
		cout << "then take " << curRoad->name << " " << curRoad->length << " miles " << direction << " to " << nextInters->name << " " << startInters->stateAbbrev << ", " << endl;
	}

	// print totatl distance
	cout << "Total Distance : " << totalW << " miles\n";
}


bool find_shortest_path(Intersection* start, Intersection* end, vector<Intersection*>& intersList)
{
	vector<PathNode*> allNodes;
	PriorityQueue queue = PriorityQueue();
	
	allNodes.push_back(new PathNode(start));
	queue.insert(new PathNode(start));
    start->visited = true;
	PathNode* minPath;
	Intersection* leadInters;
	int destN = end->num;

	while(queue.size() > 0)
	{
		// pop top off of heap
		minPath = queue.pop();
 		leadInters = minPath->intersectionP;
		
		if(leadInters->num == destN)
		{
			// found shortest path
			printMinPath(minPath);
			// draw shortest path
			draw_map(minPath, start, end);
			resetVisited(intersList);
			while(queue.size() > 0)
			{
				PathNode* p = queue.pop();
				allNodes.push_back(p);
			}
			for(int i=0;i<allNodes.size();i++)
			{
				delete allNodes[i];
			}
			return true;
		}
		// add in neighbors (exits of popped off intersection)
		for(int i=0;i<leadInters->exits.size();i++)
		{
			Road* r = leadInters->exits[i];

			Intersection* newInters;
			if(r->intA->num != leadInters->num)
			{
				newInters = r->intA;
			}
			else
			{
				newInters = r->intB;
			}
			// dont add dups to prevent loops
			//check to see if inters has been visited
			if(!intersList[newInters->num]->visited) 
			{				// insert new intersection
				newInters->visited = true;

				float newWeight = r->length + minPath->weight;
				PathNode* newNode = new PathNode(minPath,newInters,r,newWeight);
				allNodes.push_back(newNode);
				queue.insert(newNode);
			}
		}


	}
	cout << "no path found!\n";
	resetVisited(intersList);
	while(queue.size() > 0)
	{
		PathNode* p = queue.pop();
		allNodes.push_back(p);
	}
	for(int i=0;i<allNodes.size();i++)
	{
		delete allNodes[i];
	}
	return true;
}

int main(int argc, char* argv[])
{
	Hashtable<string, Place*> placesTable = Hashtable<string, Place*>();
	Hashtable<string, State*> statesTable = Hashtable<string, State*>();
	vector<Intersection*> intersList = vector<Intersection*>();
	read_places(placesTable);
	read_states(statesTable);
	read_intersections("intersections.txt", intersList);
	read_connections("connections.txt", intersList);
	for(int i = 0; i < 60; i++)
	{
			cout << endl;
	}
	cout << "Welcome to Road Trip\n";

	Intersection* nearestIntersStart;
	Intersection* nearestIntersEnd;

	cout << "Enter your starting point <Place Name> : ";
	bool runPrompt1 = prompt(placesTable, intersList, statesTable, nearestIntersStart);
	bool runPrompt2 = false;
	bool exit = false;
	while(!exit)
	{
		if(!runPrompt1)
		{

	        cout << "Enter your starting point <Place Name> : ";
			runPrompt1 = prompt(placesTable, intersList, statesTable,  nearestIntersStart);
		}
		else if(!runPrompt2)
		{

			cout << "Enter your end point <Place Name> : ";
			runPrompt2 = prompt(placesTable, intersList, statesTable, nearestIntersEnd);
		}
		else
		{
			exit = find_shortest_path(nearestIntersStart,nearestIntersEnd, intersList);
		}
	}
	
	return 0;
}

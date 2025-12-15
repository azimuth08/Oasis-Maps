#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
#include <iostream>
#include "MapStructs.h"
using namespace std;

struct PathNode
{
	PathNode* parent;
	Intersection* intersectionP;
	Road* roadP; // road that connects parent and this
	float weight;

	PathNode(PathNode* p, Intersection* neighbor, Road* r,  float w)
	{
		parent = p;
		roadP = r;
		intersectionP = neighbor;
		weight = w;
	}

	PathNode(Intersection* s)
	{
		parent = NULL;
		roadP = NULL;
		intersectionP = s;
		weight = 0.0;
	}
};

class PriorityQueue
{
	vector<PathNode*> queue;
	// helpers
	int getParent(int);
	int getLeftChild(int);
	int getRightChild(int);
        void swapUp(int,int);
	public:
	    PriorityQueue();
	    ~PriorityQueue();
	    void insert(PathNode*);
	    PathNode* getMin();
	    PathNode* pop();
	    int size();

};

#endif

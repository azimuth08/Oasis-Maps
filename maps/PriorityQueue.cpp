#include "include/PriorityQueue.h"




PriorityQueue::PriorityQueue()
{
    queue = vector<PathNode*>();
}

PriorityQueue::~PriorityQueue()
{

}

int PriorityQueue::getParent(int i)
{
    return (i-1)/2;
}

int PriorityQueue::getLeftChild(int i)
{
    return (2*i +1);
}

int PriorityQueue::getRightChild(int i)
{
    return (2*i + 2);
}

void PriorityQueue::swapUp(int parentIndex, int childIndex)
{
    if(queue[childIndex]->weight >= queue[parentIndex]->weight) return;
   
    // make the swap between parent and child
    PathNode* temp = queue[parentIndex];
    queue[parentIndex] = queue[childIndex];
    queue[childIndex] = temp;

    if(parentIndex == 0) return;
    
    int newParentIndex = getParent(parentIndex);
    swapUp(newParentIndex, parentIndex);
}

void PriorityQueue::insert(PathNode* p)
{
    queue.push_back(p);
    if(queue.size() == 1)
    {
	return;
    }
    int parentIndex = getParent(queue.size()-1);
    PathNode* parent = queue[parentIndex];
    if(p->weight < parent->weight)
    {
	swapUp(parentIndex,queue.size() -1);
    }
}

PathNode* PriorityQueue::getMin()
{
    if(queue.size() > 0)
    {
         return queue[0];
    }
    else 
    {
	return NULL;
    }
}

PathNode* PriorityQueue::pop()
{
    if(queue.empty()) return NULL;
    PathNode* min = queue[0];
    // swap top element with last and decrease heap size
    queue[0] = queue.back();
    queue.pop_back();
    int parentIndex = 0;
    int childLIndex = 1;
    int childRIndex = 2;
    while(childLIndex < queue.size())
    {
	int minChild = childLIndex;

	if(childRIndex < queue.size() && queue[childRIndex]->weight < queue[childLIndex]->weight)
	{
	    minChild = childRIndex;
	}

	if(queue[parentIndex]->weight > queue[minChild]->weight)
	{
	    // make the swap between smallest child and parent
	    PathNode* temp = queue[parentIndex];
	    queue[parentIndex] = queue[minChild];
	    queue[minChild] = temp;
	    // update position in heap
	    parentIndex = minChild;
	    childLIndex = getLeftChild(minChild);
	    childRIndex = getRightChild(minChild);
	}
	else
	{
	    break;
	}
    }

    return min;
}

int PriorityQueue::size()
{
    return queue.size();
}

#include "BinaryHeap.h"

BinaryHeap::BinaryHeap() 
{
    HeapElement he = {0,0};
    elements.push_back(he);
}


bool BinaryHeap::Add(int uniqueId, double weight)
{
    for(int i = 1; i <= HeapSize(); i++)
        if (elements[i].uniqueId == uniqueId)
            return false;
            
    HeapElement he = {uniqueId, weight};    
    elements[0] = he;        
    elements.push_back(he);   

    bubbleUp(he);
      
    return true;
}

void BinaryHeap::bubbleUp(HeapElement he)
{
    int hole = HeapSize();

    while (he.weight < elements[hole/2].weight)
    {
        elements[hole] = elements[hole/2];
        hole /= 2;
    }
    elements[hole] = he;  
}

bool BinaryHeap::PopHeap(int& outUniqueId, double& outWeight)
{
    if (HeapSize() <= 0)
        return false;

    outUniqueId = elements[1].uniqueId;
    outWeight = elements[1].weight;

    elements[1] = elements[HeapSize()];
    elements.pop_back();
    
    bubbleDown(1);
    
    return true;
}

void BinaryHeap::bubbleDown(int hole)
{
    HeapElement he = elements[hole];
    int child;
    
    while (2*hole <= HeapSize())
    {
        child = 2*hole;
        if(child != HeapSize() && elements[child+1].weight < elements[child].weight)
            child++;
        if(elements[child].weight < he.weight)
            elements[hole] = elements[child];
        else
            break;
        hole = child;
    }
    elements[hole] = he;
}

bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    for (int i = 1; i <= HeapSize(); i++)
    {
        if (elements[i].uniqueId == uniqueId)
        {
            elements[i].weight = newWeight;
            heapify();
            return true;
        }
    }
    return false;
}

void BinaryHeap::heapify()
{
    for(int i = HeapSize()/2; i > 0; i--)
        bubbleDown(i);
}

int BinaryHeap::HeapSize() const 
{
    return elements.size()-1;
}
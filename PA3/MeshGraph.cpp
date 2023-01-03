#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    for (int i = 0; i < vertexPositions.size(); i++)
    {
        Vertex v = {i, vertexPositions[i]};
        vertices.push_back(v);
    }
    for (int i = 0; i < vertexPositions.size(); i++)
    {
        std::list<Vertex*> l;
        for (int j = 0; j < edges.size(); j++)
        {
            if (edges[j].vertexId0 == i)
                l.push_back(&vertices[edges[j].vertexId1]);
            else if (edges[j].vertexId1 == i)
                l.push_back(&vertices[edges[j].vertexId0]);
        }
        adjList.push_back(l);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    if (TotalEdgeCount() == 0)
        return 0;
    else
    {
        double sum = 0;
        for (int i = 0; i < adjList.size(); i++)
            for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++)
            	sum += Double3::Distance((*it)->position3D, vertices[i].position3D);
        
        return 0.5 * sum / TotalEdgeCount();    
    }
}

double MeshGraph::AverageEdgePerVertex() const
{
    if (TotalVertexCount() == 0)
        return 0;
    else    
        return (double)TotalEdgeCount() / TotalVertexCount();
}

int MeshGraph::TotalVertexCount() const
{
    return vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int sum = 0;
    for (int i = 0; i < adjList.size(); i++)
        sum += adjList[i].size();
        
    return 0.5 * sum;    
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if (vertexId < 0 || vertexId >= vertices.size() )
        return -1;
    else
        return adjList[vertexId].size();
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    outVertexIds.resize(0);
    if (vertexId >= 0 && vertexId < vertices.size())
        for (std::list<Vertex*>::const_iterator it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++)
            outVertexIds.push_back((*it)->id);
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    int n = TotalVertexCount();
    if ((vertexIdFrom >= n) || (vertexIdFrom < 0) || (vertexIdTo >= n) || (vertexIdTo < 0))
        outputColorAllVertex.resize(0);
    else
    {
        outputColorAllVertex.resize(n);
        for (int i = 0; i < n; i++)
        {
            Color c = {0,0,0};
            outputColorAllVertex[i] = c;
        }
        
        std::vector<double> distance(n, INFINITY);
        std::vector<int> previous(n,-1);
        BinaryHeap pq;
        
        for (int i = 0; i < n; i++)
            pq.Add(i, INFINITY);
            
        pq.ChangePriority(vertexIdFrom, 0);
        distance[vertexIdFrom] = 0;
        
        int outUniqueId; double outWeight;
        
        while (pq.HeapSize() != 0)
        {    
            pq.PopHeap(outUniqueId, outWeight);
            for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); it++)
            {
                double new_distance = distance[outUniqueId] + Double3::Distance((*it)->position3D, vertices[outUniqueId].position3D);
                if (distance[(*it)->id] > new_distance)
                {
                    distance[(*it)->id] = new_distance;
                    previous[(*it)->id] = outUniqueId;
                    pq.ChangePriority((*it)->id, new_distance);
                }
            }
        }
        while (vertexIdTo != -1)
        {
            outputColorAllVertex[vertexIdTo] = color;
            if (previous[vertexIdTo] == -1)
            {
                if (vertexIdTo != vertexIdFrom)
                {
                   for (int i = 0; i < n; i++)
                    {
                        Color c = {0,0,0};
                        outputColorAllVertex[i] = c;
                    }
                    break; 
                }
            }      
            vertexIdTo = previous[vertexIdTo];
        }
    }
}

int MeshGraph::boxFilter(double x, double alpha) const
{
    if (x >= -alpha && x <= alpha)
        return 1;
    else
        return 0;
}
double MeshGraph::gaussianFilter(double x, double alpha) const
{
    double t = -(x*x)/(alpha*alpha);
    return std::exp(t);
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    if (maxDepth >= 0)
    {
        int n = TotalVertexCount();
        if (vertexId >= n || vertexId < 0)
            outputColorAllVertex.resize(0);
        else
        {
            outputColorAllVertex.resize(n);
            for (int i = 0; i < n; i++)
            {
                Color c = {0,0,0};
                outputColorAllVertex[i] = c;
            }
            
            std::vector<double> distance(n, INFINITY);
            std::vector<bool> visited(n,false);
            
            BinaryHeap q;
            
            int outUniqueId; double outWeight;
            
            for (int i = 0; i < adjList.size(); i++)
            {
                for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++)
                    q.Add((*it)->id,(*it)->id);
                for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++)
                {
                    q.PopHeap(outUniqueId, outWeight);
                    (*it)->id = outUniqueId;
                }    
            }
            
            int weight = 0;
            q.Add(vertexId, weight++); // don't forget weight++
            distance[vertexId] = 0;
            visited[vertexId] = true;
            
            while (maxDepth--)
            {
                int size = q.HeapSize();
                while (size--)
                {
                    q.PopHeap(outUniqueId, outWeight);
                    for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); it++)
                    {
                        if (!visited[(*it)->id])
                        {
                            q.Add((*it)->id, weight++);
                            distance[(*it)->id] = distance[outUniqueId] + Double3::Distance((*it)->position3D, vertices[outUniqueId].position3D);    
                            visited[(*it)->id] = true;
                        }
                    } 
                }
            }
            for (int i = 0; i < TotalVertexCount(); i++)
            {
                if (visited[i])
                {
                    double red = (double)color.r, green = (double)color.g, blue = (double)color.b;
                    if (type == FILTER_BOX)
                    {
                        Color filtered_Color = 
                        {
                            (unsigned char) (boxFilter(distance[i], alpha)*red),
                            (unsigned char) (boxFilter(distance[i], alpha)*green),
                            (unsigned char) (boxFilter(distance[i], alpha)*blue)
                        };
                        outputColorAllVertex[i] = filtered_Color;   
                    }
                    else if (type == FILTER_GAUSSIAN)
                    {
                        Color filtered_Color = 
                        {
                            (unsigned char) (gaussianFilter(distance[i], alpha)*red),
                            (unsigned char) (gaussianFilter(distance[i], alpha)*green),
                            (unsigned char) (gaussianFilter(distance[i], alpha)*blue)
                        };
                        outputColorAllVertex[i] = filtered_Color;   
                    }
                }
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    if (maxDepth >= 0)
    {
        int n = TotalVertexCount();
        if (vertexId >= n || vertexId < 0)
            outputColorAllVertex.resize(0);
        else
        {
            outputColorAllVertex.resize(n);
            for (int i = 0; i < n; i++)
            {
                Color c = {0,0,0};
                outputColorAllVertex[i] = c;
            }
            
            std::vector<double> distance(n, INFINITY);
            for (int i = 0; i < n; i++)
                distance[i] = Double3::Distance(vertices[vertexId].position3D, vertices[i].position3D);
        
            std::vector<bool> visited(n,false);
            
            BinaryHeap q;
            
            int outUniqueId; double outWeight;
            
            for (int i = 0; i < adjList.size(); i++)
            {
                for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++)
                    q.Add((*it)->id,(*it)->id);
                for (std::list<Vertex*>::const_iterator it = adjList[i].begin(); it != adjList[i].end(); it++)
                {
                    q.PopHeap(outUniqueId, outWeight);
                    (*it)->id = outUniqueId;
                }    
            }
            
            int weight = 0;
            q.Add(vertexId, weight++); // don't forget weight++
            distance[vertexId] = 0;
            visited[vertexId] = true;
            
            while (maxDepth--)
            {
                int size = q.HeapSize();
                while (size--)
                {
                    q.PopHeap(outUniqueId, outWeight);
                    for (std::list<Vertex*>::const_iterator it = adjList[outUniqueId].begin(); it != adjList[outUniqueId].end(); it++)
                    {
                        if (!visited[(*it)->id])
                        {
                            q.Add((*it)->id, weight++);
                            visited[(*it)->id] = true;
                        }
                    } 
                }
            }
            for (int i = 0; i < TotalVertexCount(); i++)
            {
                if (visited[i])
                {
                    double red = (double)color.r, green = (double)color.g, blue = (double)color.b;
                    if (type == FILTER_BOX)
                    {
                        Color filtered_Color = 
                        {
                            (unsigned char) (boxFilter(distance[i], alpha)*red),
                            (unsigned char) (boxFilter(distance[i], alpha)*green),
                            (unsigned char) (boxFilter(distance[i], alpha)*blue)
                        };
                        outputColorAllVertex[i] = filtered_Color;   
                    }
                    else if (type == FILTER_GAUSSIAN)
                    {
                        Color filtered_Color = 
                        {
                            (unsigned char) (gaussianFilter(distance[i], alpha)*red),
                            (unsigned char) (gaussianFilter(distance[i], alpha)*green),
                            (unsigned char) (gaussianFilter(distance[i], alpha)*blue)
                        };
                        outputColorAllVertex[i] = filtered_Color;   
                    }
                }
            }
        }
    }
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}

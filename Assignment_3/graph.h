#ifndef _GRAPH_H
#define _GRAPH_H
#define INFINITY 2000000000
#include <list>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include "hash.h"
#include <climits>
#include <vector>
#include <string>
#include "heap.h"

class Graph {
class Edge;
class Node;

private:
class Edge {
public:
Node* dest;
int cost;
};

class Node {
public:
std::string name;
int dist;
Node *prev;
bool known;
std::vector<Edge> adjacent;

};

std::string s_vertex;
std::list<Node *> listOfNodes;
hashTable *nodeHashTable;


public:
Graph();
int loadGraph(std::string, std::string);
void Dijkstra(std::string &startingNode);
// void exportFile(std::ofstream &out);
void exportFile(std::string outFile);
void printPath(Node * startnode, std::ofstream &file);


};

#endif

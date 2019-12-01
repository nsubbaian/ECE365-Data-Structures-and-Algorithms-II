#include "graph.h"
#include <cstring>



Graph::Graph(){
        nodeHashTable = new hashTable(50000); //What should I initialize this with?
}

int Graph::loadGraph(std::string inputFile, std::string startingNode){

        std::ifstream file;
        file.open(inputFile);

        std::string line;
        while(getline(file, line)) {

                std::string firstNode, destinationNode;
                int weight;

                std::stringstream ss(line);
                ss >> firstNode >> destinationNode >> weight;

                Node *startNode, *endNode;

                if(nodeHashTable->contains(firstNode)) {
                        startNode = (Node *) nodeHashTable->getPointer(firstNode);
                } else {
                        startNode = new Node;
                        startNode->name = firstNode;
                        startNode->dist = INFINITY;
                        startNode->prev = NULL;
                        startNode->known = false;
                        listOfNodes.push_back(startNode);
                        nodeHashTable->insert(firstNode, startNode);
                }

                if(nodeHashTable->contains(destinationNode)) {
                        endNode = (Node *) nodeHashTable->getPointer(destinationNode);
                } else {
                        endNode = new Node;
                        endNode->name = destinationNode;
                        endNode->dist = INFINITY;
                        endNode->prev = NULL;
                        endNode->known = false;
                        listOfNodes.push_back(endNode);
                        nodeHashTable->insert(destinationNode, endNode);
                }

                Edge edge;
                edge.dest = endNode;
                edge.cost = weight;
                startNode->adjacent.push_back(edge);

        }

        file.close();

        if(!nodeHashTable->contains(startingNode)) {
                std::cout << "This Node does not Exist, Enter a new node to be the starting node\n";
                return 1;
        }

        return 0;
}


void Graph::Dijkstra(std::string &startingNode) {
        Node *startingNode1 = new Node;
        startingNode1 = (Node *) nodeHashTable->getPointer(startingNode);
        startingNode1->dist = 0;
        heap dijkstraHeap(listOfNodes.size());
        Node *temp;

        for (std::list<Node *>::const_iterator it = listOfNodes.begin(), end = listOfNodes.end(); it != end; ++it) {
                dijkstraHeap.insert((*it)->name, (*it)->dist, *it);
        }
        int edgeCount = 0;
        while(!dijkstraHeap.deleteMin(nullptr, nullptr, &temp)) {

                for (std::vector<Edge>::const_iterator it = temp->adjacent.begin(), end = temp->adjacent.end();
                     it != end && temp->dist != INFINITY; ++it) {

                       	Node *temp2 = (Node *) nodeHashTable -> getPointer(temp -> adjacent[edgeCount].dest->name);
                        if (!it->dest->known && it->dest->dist > (it->cost + temp->dist)) {
                                temp2 -> prev = temp;
                                it->dest->dist = (it->cost + temp->dist);
                                dijkstraHeap.setKey(it->dest->name, (it->cost + temp->dist));
                        }
                        edgeCount++;
                }
                edgeCount = 0;
                temp->known = true;
        }
}


void Graph::exportFile(std::string outFile) {
        std::ofstream filee;
        filee.open(outFile);
        std::list<Node *>::const_iterator it;

        for(it = listOfNodes.begin(); it != listOfNodes.end(); it++) {
                if((*it)->dist == INFINITY)
                        filee << (*it)->name << ": NO PATH" << std::endl;
                else{
                        filee << (*it)->name << ": ";
                        filee << (*it)->dist << " [";
                        printPath(*it, filee);
                        filee << "]" << std::endl;
                }
        }
        filee.close();
}

void Graph::printPath(Node* startnode, std::ofstream &file){
        if(startnode->prev != NULL) {
                printPath(startnode->prev, file);
                file << ", ";
        }
        file << startnode->name;

}

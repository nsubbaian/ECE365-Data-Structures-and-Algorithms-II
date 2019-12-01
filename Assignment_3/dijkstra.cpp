
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <climits>
#include "graph.h"

int main() {

        std::string inFile, startingNode, outFile;
        clock_t timeStart, timebuffer, timeEnd;
        Graph loadedgraph;

        std::cout << "Enter name of graph file: ";
        std::cin >> inFile;
        std::cout << "Enter a valid vertex id for the staring vertex: ";
        std::cin >> startingNode;

        timebuffer = clock();
        timeStart = timebuffer;
        while(loadedgraph.loadGraph(inFile, startingNode)) {
                std::cout << "Please specify the starting node: ";
                std::cin >> startingNode;
        }

        loadedgraph.Dijkstra(startingNode);

        timeEnd = clock();
        float d_time = (float)(timeEnd - timeStart) / (float)CLOCKS_PER_SEC;
        std::cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << d_time << std::endl;

        std::cout << "Enter name of output file: ";
        std::cin >> outFile;
        loadedgraph.exportFile(outFile);

        return 0;
}

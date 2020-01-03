//
// Created by andreea on 02.12.2019.
//

#include <fstream>
#include <iostream>
#include "directed_graph.h"

directed_graph::directed_graph(const std::string& filename) {
    std::ifstream fin;
    fin.open(filename);
    int start, stop;
    fin >> n;
    edges = std::vector<std::vector<int> >((unsigned int)n, std::vector<int>());
    transposed_edges = std::vector<std::vector<int> >((unsigned int)n, std::vector<int>());
    while(fin >> start >> stop) {
        edges[start].push_back(stop);
        transposed_edges[stop].push_back(start);
    }
    fin.close();
}

void directed_graph::printEdges() {
    for(int vertex = 0; vertex < n; vertex++) {
        std::cout << vertex << " : ";
        for (auto el: edges[vertex])
            std::cout << el << " ";
        std::cout << '\n';
    }
}

std::vector<int> directed_graph::getNeighbours(int node) {
    return edges[node];
}

std::vector<int> directed_graph::getTransposedNeighbours(int node) {
    return transposed_edges[node];
}



std::vector<int> findHamiltonianCycle () {
    std::vector<int> result;

    return result;
}
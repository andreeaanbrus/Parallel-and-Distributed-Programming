//
// Created by andreea on 02.12.2019.
//

#ifndef LAB8_DIRECTED_GRAPH_H
#define LAB8_DIRECTED_GRAPH_H
#include <string>
#include <vector>

class directed_graph {
public:
    std::vector<std::vector<int>> edges ;
    std::vector<std::vector<int>> transposed_edges ;
    int n;
    directed_graph(const std::string& filename);
    std::vector<int> getNeighbours(int node);
    std::vector<int> getTransposedNeighbours(int node);
        void printEdges();
};


#endif //LAB8_DIRECTED_GRAPH_H

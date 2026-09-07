#include <iostream>
#include "graph.h"

using namespace std;

int main() {

    int matrix[9][9] = {
        {0, 2, 1, 7, 0, 0, 0, 0, 0},
        {2, 0, 5, 5, 0, 0, 0, 0, 0},
        {1, 5, 0, 4, 0, 9, 0, 0, 0},
        {7, 5, 4, 0, 8, 0, 0, 0, 0},
        {0, 0, 0, 8, 0, 3, 7, 0, 0},
        {0, 0, 9, 0, 3, 0, 5, 10, 0},
        {0, 0, 0, 0, 7, 5, 0, 11, 6},
        {0, 0, 0, 0, 0, 10, 11, 0, 3},
        {0, 0, 0, 0, 0, 0, 6, 3, 0}
    };

    Graph g(matrix);

    g.showNodes();
    g.showEdges();

    cout << boolalpha << endl;

    cout << "Multigraph: " << g.isMultigraph() << endl;
    cout << "Pseudograph: " << g.isPseudograph() << endl;
    cout << "Digraph: " << g.isDigraph() << endl;
    cout << "Weighted graph: " << g.isWeighted() << endl;
    cout << "Complete graph: " << g.isComplete() << endl;
    cout << "Disjointed graph: " << g.isDisjointed() << endl;

    return 0;
}
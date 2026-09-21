#ifndef GRAPH_H
#define GRAPH_H
 
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <map>
#include <limits>
 
using namespace std;
 
class Node {
public:
    char label;
    Node(char name) : label(name) {}
};
 
class Edge {
public:
    Node* start;
    Node* end;
    int weight;
    Edge(Node* s, Node* e, int w) : start(s), end(e), weight(w) {}
};
 
class Graph {
public:
    static const int SIZE = 9;
 
private:
    list<Node> nodes;
    list<Edge> edges;
 
public:
 
    Graph(int data[SIZE][SIZE]) {
        Node* nodeList[SIZE];
 
        for (int i = 0; i < SIZE; ++i) {
            nodes.emplace_back('A' + i);
            nodeList[i] = &nodes.back();
        }
 
        for (int row = 0; row < SIZE; ++row) {
            for (int col = 0; col < SIZE; ++col) {
                if (data[row][col] == 0)
                    continue;
                edges.emplace_back(nodeList[row], nodeList[col], data[row][col]);
            }
        }
    }
 
    void showNodes() {
        cout << "Nodes: ";
        for (const Node& node : nodes)
            cout << node.label << " ";
        cout << '\n';
    }
 
    void showEdges() {
        cout << "Edges:" << '\n';
        for (const Edge& edge : edges) {
            cout << edge.start->label << " -> " << edge.end->label
                 << " weight " << edge.weight << '\n';
        }
    }
 
    bool isMultigraph() {
        for (auto first = edges.begin(); first != edges.end(); ++first) {
            for (auto second = next(first); second != edges.end(); ++second) {
                if (first->start == second->start && first->end == second->end)
                    return true;
            }
        }
        return false;
    }
 
    bool isPseudograph() {
        for (const Edge& edge : edges) {
            if (edge.start == edge.end)
                return true;
        }
        return false;
    }
 
    bool isDigraph() {
        for (const Edge& edge : edges) {
            bool hasOpposite = false;
            for (const Edge& other : edges) {
                if (edge.start == other.end && edge.end == other.start &&
                    edge.weight == other.weight) {
                    hasOpposite = true;
                    break;
                }
            }
            if (!hasOpposite)
                return true;
        }
        return false;
    }
 
    bool isWeighted() {
        for (const Edge& edge : edges) {
            if (edge.weight != 1)
                return true;
        }
        return false;
    }
 
    bool isComplete() {
        for (auto first = nodes.begin(); first != nodes.end(); ++first) {
            for (auto second = next(first); second != nodes.end(); ++second) {
                bool found = false;
                for (const Edge& edge : edges) {
                    bool forward = edge.start == &(*first) && edge.end == &(*second);
                    bool backward = edge.start == &(*second) && edge.end == &(*first);
                    if (forward || backward) {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    return false;
            }
        }
        return true;
    }
 
    bool isDisjointed() {
        for (const Node& node : nodes) {
            bool hasConnection = false;
            for (const Edge& edge : edges) {
                if (edge.start == &node || edge.end == &node) {
                    hasConnection = true;
                    break;
                }
            }
            if (!hasConnection)
                return true;
        }
        return false;
    }
 
    // Minimum Spanning Tree - Prim's algorithm
    list<Edge> getMST() {
        list<Edge> mst;
        if (nodes.empty())
            return mst;
 
        vector<Node*> visited;
        visited.push_back(&nodes.front());
 
        while (visited.size() < nodes.size()) {
            Edge* cheapest = nullptr;
 
            for (Edge& edge : edges) {
                bool startVisited = find(visited.begin(), visited.end(), edge.start) != visited.end();
                bool endVisited = find(visited.begin(), visited.end(), edge.end) != visited.end();
 
                if (startVisited && !endVisited) {
                    if (cheapest == nullptr || edge.weight < cheapest->weight)
                        cheapest = &edge;
                }
            }
 
            if (cheapest == nullptr)
                break;
 
            mst.push_back(*cheapest);
            visited.push_back(cheapest->end);
        }
 
        return mst;
    }
 
    void showMST() {
        list<Edge> mst = getMST();
        int totalWeight = 0;
 
        cout << "Minimum Spanning Tree:" << '\n';
        for (const Edge& edge : mst) {
            cout << edge.start->label << " -> " << edge.end->label
                 << " weight " << edge.weight << '\n';
            totalWeight += edge.weight;
        }
        cout << "Total weight: " << totalWeight << '\n';
    }
 
    // Shortest paths - Dijkstra's algorithm
    void showShortestPaths(char startLabel) {
        Node* start = nullptr;
        for (Node& node : nodes) {
            if (node.label == startLabel) {
                start = &node;
                break;
            }
        }
 
        if (start == nullptr) {
            cout << "Node " << startLabel << " not found." << '\n';
            return;
        }
 
        const int INF = numeric_limits<int>::max();
        map<Node*, int> distance;
        map<Node*, Node*> previous;
        vector<Node*> unvisited;
 
        for (Node& node : nodes) {
            distance[&node] = INF;
            previous[&node] = nullptr;
            unvisited.push_back(&node);
        }
        distance[start] = 0;
 
        while (!unvisited.empty()) {
            auto currentIt = unvisited.begin();
            for (auto it = unvisited.begin(); it != unvisited.end(); ++it) {
                if (distance[*it] < distance[*currentIt])
                    currentIt = it;
            }
 
            Node* current = *currentIt;
            unvisited.erase(currentIt);
 
            if (distance[current] == INF)
                break;
 
            for (Edge& edge : edges) {
                if (edge.start != current)
                    continue;
                int newDist = distance[current] + edge.weight;
                if (newDist < distance[edge.end]) {
                    distance[edge.end] = newDist;
                    previous[edge.end] = current;
                }
            }
        }
 
        cout << "Shortest paths from " << start->label << ":" << '\n';
        for (Node& node : nodes) {
            cout << start->label << " -> " << node.label << " : ";
 
            if (distance[&node] == INF) {
                cout << "unreachable" << '\n';
                continue;
            }
 
            cout << "distance " << distance[&node] << ", path: ";
 
            vector<Node*> path;
            Node* step = &node;
            while (step != nullptr) {
                path.push_back(step);
                step = previous[step];
            }
 
            for (auto it = path.rbegin(); it != path.rend(); ++it) {
                cout << (*it)->label;
                if (next(it) != path.rend())
                    cout << " -> ";
            }
            cout << '\n';
        }
    }
};
 
#endif
 



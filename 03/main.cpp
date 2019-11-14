#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-too-small-loop-variable"
#pragma ide diagnostic ignored "cert-err34-c"

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// T is the number of the input graphs
// N is the number of vertices of each graph
// M and is the number of edges of each graph
int T, N, M;

struct Node {
    string certificate = "C";
    bool in_cycle = false;
    list<int> adjacent;
};

struct Graph {
    vector<Node> nodes;
    string certificate;
};

vector<Graph> graphs;

int main() {
    fscanf(stdin, "%d %d %d", &T, &N, &M);
    graphs = vector<Graph>(T);

    for (auto &g : graphs) {
        g.nodes = vector<Node>(N);

        for (auto i = 0; i < M; i++) {
            int from, to;
            fscanf(stdin, "%d %d", &from, &to);
            int from_idx = from - 1;
            int to_idx = to - 1;

            g.nodes[from_idx].adjacent.push_back(to_idx);
            g.nodes[to_idx].adjacent.push_back(from_idx);
        }
    }


    return 0;

}
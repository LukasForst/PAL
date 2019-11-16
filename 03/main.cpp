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

enum ColoringState {
    NEW, PARTIALLY_VISITED, COMPLETELY_VISITED
};

struct Node {
    string certificate = "C";
    bool in_cycle = false;
    bool removed = false;
    ColoringState color = NEW;
    list<int> adjacent;
};

struct Graph {
    vector<Node> nodes;
    string certificate;
};

vector<Graph> graphs;

void dfs_cycle(int u, int p, vector<int> &color, vector<int> &par, vector<Node> &nodes) {

    auto &current = nodes[u];
    // already (completely) visited vertex.
    if (current.color == COMPLETELY_VISITED) {
        return;
    }

    // seen vertex, but was not completely visited -> cycle detected.
    // backtrack based on parents to find the complete cycle.
    if (current.color == PARTIALLY_VISITED) {
        int cur = p;
        nodes[cur].in_cycle = true;
        // backtrack the vertex which are
        // in the current cycle thats found
        while (cur != u) {
            cur = par[cur];
            nodes[cur].in_cycle = true;
        }
        return;

    } else {
        par[u] = p;

        // partially visited.
        current.color = PARTIALLY_VISITED;

        // simple dfs on graph
        for (int v : current.adjacent) {

            // if it has not been visited previously
            if (v == par[u]) {
                continue;
            }

            dfs_cycle(v, u, color, par, nodes);
        }

        // completely visited.
        current.color = COMPLETELY_VISITED;
    }
}


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

        vector<int> color(N);
        vector<int> parents(N);
        dfs_cycle(0, -1,color, parents, g.nodes);
        cout << "Nodes in cycle: ";
        for(int i = 0; i < N; i++) {
            if(g.nodes[i].in_cycle) {
                cout << i << ", ";
            }
        }
        cout << endl;
    }


    return 0;

}
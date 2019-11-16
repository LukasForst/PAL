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
    ColoringState color = NEW;
    list<int> adjacent;
};

struct Graph {
    vector<Node> nodes;
    int root;
    string certificate;
};

vector<Graph> graphs;

void dfs_cycle(int u, int p, vector<int> &color, vector<int> &par, vector<Node> &nodes) {
    auto &current = nodes[u];
    if (current.color == PARTIALLY_VISITED) {
        int cur = p;
        nodes[cur].in_cycle = true;

        while (cur != u) {
            cur = par[cur];
            nodes[cur].in_cycle = true;
        }
    } else {
        par[u] = p;
        current.color = PARTIALLY_VISITED;

        for (auto &v : current.adjacent) {
            if (v != par[u]) {
                dfs_cycle(v, u, color, par, nodes);
            }
        }

        current.color = COMPLETELY_VISITED;
    }
}

void reset_state(Graph &g) {
    for (auto &n : g.nodes) {
        n.color = NEW;
    }
}

void print_cycles(Graph &g) {
    cout << "Nodes in cycle: ";
    for (int i = 0; i < N; i++) {
        if (g.nodes[i].in_cycle) {
            cout << i << ", ";
        }
    }
    cout << endl;
}

void find_cycles(Graph &g) {
    vector<int> color(N);
    vector<int> parents(N);
    dfs_cycle(0, -1, color, parents, g.nodes);
}

void find_root(Graph &g) {
    for (auto i = 0; i < N; i++) {
        if (!g.nodes[i].in_cycle && g.nodes[i].adjacent.size() == 3) {
            g.root = i;
            break;
        }
    }
}

string find_node_certificate(int p, int c, vector<Node> &nodes) {
    auto &parent = nodes[p];
    auto &current = nodes[c];


    return "";
}

void find_graph_certificate(Graph &g) {
    vector<string> certificates(3);

    auto &root = g.nodes[g.root];
    int idx = 0;
    for (auto i: root.adjacent) {
        certificates[idx++] = find_node_certificate(g.root, i, g.nodes);
    }
    sort(certificates.begin(), certificates.end());

    g.certificate = certificates[0] + "A" + certificates[1] + "B" + certificates[2];
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

        find_cycles(g);
        print_cycles(g);
        find_root(g);
        cout << "Root: " << g.root << endl;

        reset_state(g);

    }


    return 0;

}
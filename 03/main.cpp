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
#include <set>

#define DEBUG 1

using namespace std;

// T is the number of the input graphs
// N is the number of vertices of each graph
// M and is the number of edges of each graph
int T, N, M;

enum ColoringState {
    NEW, PARTIALLY_VISITED, COMPLETELY_VISITED
};

struct Node {
    string certificate = "L";
    int cycle_id = -1;
    ColoringState color = NEW;
    list<int> adjacent;
};

struct Graph {
    vector<Node> nodes;
    int root = -1;
    string certificate = "";
};

void dfs_cycle(int u, int p, vector<int> &par, vector<Node> &nodes, int &cycle_counter) {
    auto &current = nodes[u];

    if (current.color == NEW) {
        par[u] = p;
        current.color = PARTIALLY_VISITED;

        for (auto &v : current.adjacent) {
            if (v != par[u]) {
                dfs_cycle(v, u, par, nodes, cycle_counter);
            }
        }

        current.color = COMPLETELY_VISITED;
    } else if (current.color == PARTIALLY_VISITED) {
        int cur = p;
        cycle_counter++;

        nodes[cur].cycle_id = cycle_counter;

        while (cur != u) {
            cur = par[cur];
            nodes[cur].cycle_id = cycle_counter;
            nodes[cur].certificate = "C";
        }
    }
}

void reset_state(Graph &g) {
    for (auto &n : g.nodes) {
        n.color = NEW;
    }
}

void print_cycles(Graph &g) {
    cout << "Nodes in cycle: ";
    for (auto i = 0; i < N; i++) {
        if (g.nodes[i].cycle_id != -1) {
            cout << i + 1 << " (" << g.nodes[i].cycle_id << "),";
        }
    }
    cout << endl;
}

void find_cycles(Graph &g) {
    vector<int> parents(N);
    int cycle_counter = 0;
    dfs_cycle(0, -1, parents, g.nodes, cycle_counter);
}

void find_root(Graph &g) {
    for (auto i = 0; i < N; i++) {
        if (g.nodes[i].cycle_id == -1 && g.nodes[i].adjacent.size() == 3) {
            g.root = i;
            break;
        }
    }
}

string find_node_certificate(int p, int c, vector<Node> &nodes) {
    auto &parent = nodes[p];
    auto &current = nodes[c];

    if (current.adjacent.size() == 1) {
        return current.certificate;
    } else if (current.color == PARTIALLY_VISITED) {
        return "";
    }

    current.color = PARTIALLY_VISITED;

    // different cycle only
    for (auto adj: current.adjacent) {
        auto &next = nodes[adj];
        if (adj == p || next.cycle_id == current.cycle_id) continue;
        current.certificate = current.certificate + "{" + find_node_certificate(c, adj, nodes) + "}";
    }

    // same cycle only
    for (auto adj: current.adjacent) {
        auto &next = nodes[adj];
        if (adj == p || next.cycle_id != current.cycle_id) continue;

        current.certificate += find_node_certificate(c, adj, nodes);
    }

    current.color = COMPLETELY_VISITED;
    return current.certificate;
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

void print_result(vector<Graph> &graphs, set<string> &certificates) {
    auto size = certificates.size();

    vector<int> counts(size);
    int idx = 0;
    for (auto &c : certificates) {
        int count = 0;
        for (auto &g: graphs) {
            if (g.certificate == c) {
                count++;
            }
        }
        counts[idx++] = count;
    }

    for (auto i = 0; i < size; i++) {
        cout << counts[i];
        if (i != size - 1) {
            cout << " ";
        }
    }
    cout << endl;
}


int main() {
    fscanf(stdin, "%d %d %d", &T, &N, &M);
    vector<Graph> graphs(T);
    set<string> certificates;

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
        find_root(g);

        if (DEBUG) {
            print_cycles(g);
            cout << "Root: " << g.root + 1 << endl;
        }

        reset_state(g);

        find_graph_certificate(g);
        certificates.insert(g.certificate);
    }

    print_result(graphs, certificates);
    return 0;
}
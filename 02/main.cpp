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
#include <algorithm>    // std::sort

using namespace std;

struct Node {
    list<int> adjacent;
    int component = -1;
    int price = 1;
    int idx;
    bool on_stack = false;
    int disc = -1;
    int lowlink = INT32_MAX;
};

int N, M;

vector<Node> adjacency_list;

void remove_inner_component_edges(vector<Node> &nodes, list<int> &nodes_with_edges) {
    for (auto i = 0; i < nodes.size(); i++) {
        auto &node = nodes[i];

        list<int> to_remove;

        for (auto adjacent : node.adjacent) {
            auto &child = nodes[adjacent];

            if (child.component == node.component || child.price < node.price) {
                to_remove.push_back(adjacent);
            }
        }

        for (auto &to_remove_node : to_remove) {
            node.adjacent.remove(to_remove_node); // TODO this can be optimized
        }

        if (!node.adjacent.empty()) {
            nodes_with_edges.push_back(i);
        }
    }
}


struct express_path {
    int price;
    int lenght;
};

express_path path_find(int idx, int depth, int price) {
    auto &current = adjacency_list[idx];

    price += current.price;
    express_path best_node{price, depth};

    for (auto adjacent: current.adjacent) {
        auto child_node = path_find(adjacent, depth + 1, price);

        if (best_node.price < child_node.price ||
            (best_node.price == child_node.price && best_node.lenght < child_node.lenght)) {
            best_node = child_node;
        }

    }

    return best_node;
}

express_path find_path() {
    list<int> non_empty_nodes;
    remove_inner_component_edges(adjacency_list, non_empty_nodes);


    express_path result{0, 0};
    for (int i = 0; i < adjacency_list.size(); i++) {

        if (adjacency_list[i].adjacent.empty()) continue;

        auto child_result = path_find(i, 0, 0);

//        cout << i << " " << child_result.price << " " << child_result.lenght << "\n";

        if (result.price < child_result.price ||
            (result.price == child_result.price && result.lenght < child_result.lenght)) {
            result = child_result;
        }
    }

    return result;
}

stack<int> traversal_stack;

void push(int i) {
    adjacency_list[i].on_stack = true;
    traversal_stack.push(i);
}

void pop(int i) {
    adjacency_list[i].on_stack = false;
    traversal_stack.pop();
}

int top() {
    return traversal_stack.top();
}

int take() {
    int i = top();
    pop(i);
    return i;
}

int idx_counter = 0;

int generate_idx() {
    return idx_counter++;
}

int component_counter = 0;
int generate_component_number() {
    return component_counter++;
}

// A recursive function that finds and prints strongly connected
// components using DFS traversal
// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store all the connected ancestors (could be part
//           of SCC)
// stackMember[] --> bit/index array for faster check whether
//                  a node is in stack
void ssc(int u, int disc[], int low[]) {

    disc[u] = low[u] = generate_idx();
    push(u);

    // Go through all vertices adjacent to this
    for (auto &v: adjacency_list[u].adjacent) {

        if (disc[v] == -1) {
            ssc(v, disc, low);

            low[u] = min(low[u], low[v]);
        } else if (adjacency_list[v].on_stack) {
            low[u] = min(low[u], disc[v]);

        }
    }

    int w = 0;  // To store stack extracted vertices
    if (low[u] == disc[u]) {
        list<int> components;

        while (top() != u) {
            w = take();
            components.push_back(w);
        }

        components.push_back(take());

        auto size = components.size();
        auto component_no = generate_component_number();
        for (const auto &i: components) {
            adjacency_list[i].price = size;
            adjacency_list[i].component = component_no;
        }
    }
}

void tarjan() {
    int *disc = new int[N];
    int *low = new int[N];

    for (int i = 0; i < N; i++) {
        disc[i] = -1;
        low[i] = INT32_MAX;
    }

    for (int i = 0; i < N; i++)
        if (disc[i] == -1)
            ssc(i, disc, low);
}

int main() {
    fscanf(stdin, "%d %d", &N, &M);
    adjacency_list = vector<Node>(N);

    for (auto i = 0; i < M; i++) {
        if (i < N) {
            adjacency_list[i].idx = i;
        }

        int from, to;
        fscanf(stdin, "%d %d", &from, &to);

        adjacency_list[from].adjacent.push_back(to);
    }

    tarjan();
    auto result = find_path();
    cout << result.price << " " << result.lenght << endl;

    return 0;
}
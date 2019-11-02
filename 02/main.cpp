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

void init_search_round(int v) {
    auto &V = adjacency_list[v];
    auto idx = generate_idx();
    V.lowlink = idx;
    V.disc = idx;
    push(v);
}


void component_found(int u) {
    list<int> components;

    while (top() != u) {
        components.push_back(take());
    }

    components.push_back(take());

    auto size = components.size();
    auto component_no = generate_component_number();
    for (const auto &i: components) {
        adjacency_list[i].price = size;
        adjacency_list[i].component = component_no;
    }

}

void ssc(int u) {

    init_search_round(u);
    auto &U = adjacency_list[u];

    for (auto &v: adjacency_list[u].adjacent) {
        auto &V = adjacency_list[v];

        if (V.disc == -1) {
            ssc(v);
            U.lowlink = min(U.lowlink, V.lowlink);
        } else if (V.on_stack) {
            U.lowlink = min(U.lowlink, V.disc);
        }
    }

    if (U.lowlink == U.disc) {
        component_found(u);
    }
}

void tarjan() {
    for (int i = 0; i < N; i++) {
        if (adjacency_list[i].disc != -1) {
            continue;
        }
        ssc(i);
    }

}

int main() {
    fscanf(stdin, "%d %d", &N, &M);
    adjacency_list = vector<Node>(N);

    for (auto i = 0; i < M; i++) {
        if (i < N) {
            adjacency_list[i].idx = i;
            adjacency_list[i].disc = -1;
            adjacency_list[i].lowlink = INT32_MAX;
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
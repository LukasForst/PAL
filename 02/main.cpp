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
};

int N, M;

vector<Node> adjacency_list;
vector<Node> reversed_adjacency_list;


string join_list(list<int> &ids) {
    string result;
    for (int i : ids) {
        result += "," + to_string(i);
    }
    return result;
}

void print(vector<Node> &nodes) {
    cout << "Idx - Component - price " << endl;
    for (auto i = 0; i < nodes.size(); i++) {
        auto &n = nodes[i];

        cout << i << " - " << n.component << " - " << n.price << " ---- " << join_list(n.adjacent) << "\n" << endl;
    }
}

bool srt(const Node &n1, const Node &n2) {
    return n1.component < n2.component;
}

void print_components(vector<Node> nodes){
    sort(nodes.begin(), nodes.end(), srt);
    int previous = -2;
    for (auto &n : nodes) {
        if(previous == n.component) continue;
        previous = n.component;

        list<int> same;
        for(auto &n2 : nodes) {
            if(n2.component == previous) {
                same.push_back(n2.idx);
            }
        }

        cout << n.component << " - " << join_list(same) << "\n";
    }
}


void dfs_walk(int current, vector<bool> &explored, stack<int> &visited_stack) {
    explored[current] = true;

    for (const auto &adjacent : adjacency_list[current].adjacent) {
        if (!explored[adjacent]) {
            dfs_walk(adjacent, explored, visited_stack);
        }
    }

    visited_stack.push(current);
}

void reverse_dfs_walk(int current, vector<bool> &explored, list<int> &component) {
    explored[current] = true;
    component.push_back(current);

    for (const auto &adjacent : reversed_adjacency_list[current].adjacent) {
        if (!explored[adjacent]) {
            reverse_dfs_walk(adjacent, explored, component);
        }
    }
}

void clean_boolean_vector(vector<bool> &d) {
    for (auto &&i : d) {
        i = false;
    }
}

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

express_path run() {
    stack<int> stack;
    vector<bool> explored(N);

    clean_boolean_vector(explored);
    dfs_walk(0, explored, stack);
    clean_boolean_vector(explored);

    int current_component = 0;
    while (!stack.empty()) {
        auto &current_node = stack.top();
        stack.pop();

        if (explored[current_node]) continue;

        list<int> component {};
        reverse_dfs_walk(current_node, explored,component);

        const auto size = component.size();
        for (auto &node_idx : component) {
            adjacency_list[node_idx].price = size;
            adjacency_list[node_idx].component = current_component;
        }

        current_component++;
    }

    list<int> non_empty_nodes;
    remove_inner_component_edges(adjacency_list, non_empty_nodes);


    print_components(adjacency_list);
//    print(adjacency_list);

    express_path result{0, 0};
    for (int i = 0; i < adjacency_list.size(); i++) {

        if(adjacency_list[i].adjacent.empty()) continue;

        auto child_result = path_find(i, 0, 0);

//        cout << i << " " << child_result.price << " " << child_result.lenght << "\n";

        if (result.price < child_result.price ||
            (result.price == child_result.price && result.lenght < child_result.lenght)) {
            result = child_result;
        }
    }

    return result;
}

int main() {
    fscanf(stdin, "%d %d", &N, &M);
    adjacency_list = vector<Node>(N);
    reversed_adjacency_list = vector<Node>(N);

    for (auto i = 0; i < M; i++) {
        if(i < N) {
            adjacency_list[i].idx = i;
        }

        int from, to;
        fscanf(stdin, "%d %d", &from, &to);

        adjacency_list[from].adjacent.push_back(to);
        reversed_adjacency_list[to].adjacent.push_back(from);
    }

    auto result = run();
    cout << result.price << " " << result.lenght << endl;
    return 0;
}
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
void ssc(int u, int disc[], int low[], stack<int> *st,
                    bool stackMember[])
{
    // A static variable is used for simplicity, we can avoid use
    // of static variable by passing a pointer.
    static int time = 0;

    // Initialize discovery time and low value
    disc[u] = low[u] = ++time;
    st->push(u);
    stackMember[u] = true;

    // Go through all vertices adjacent to this
    list<int>::iterator i;
    for (i = adjacency_list[u].adjacent.begin(); i != adjacency_list[u].adjacent.end(); ++i)
    {
        int v = *i;  // v is current adjacent of 'u'

        // If v is not visited yet, then recur for it
        if (disc[v] == -1)
        {
            ssc(v, disc, low, st, stackMember);

            // Check if the subtree rooted with 'v' has a
            // connection to one of the ancestors of 'u'
            // Case 1 (per above discussion on Disc and Low value)
            low[u]  = min(low[u], low[v]);
        }

            // Update low value of 'u' only of 'v' is still in stack
            // (i.e. it's a back edge, not cross edge).
            // Case 2 (per above discussion on Disc and Low value)
        else if (stackMember[v])
            low[u]  = min(low[u], disc[v]);
    }

    static int component_no = 0;
    // head node found, pop the stack and print an SCC
    int w = 0;  // To store stack extracted vertices
    if (low[u] == disc[u])
    {
        list<int> components;
        while (st->top() != u)
        {
            w = (int) st->top();
            components.push_back(w);
            stackMember[w] = false;
            st->pop();
        }
        w = (int) st->top();
        components.push_back(w);
        stackMember[w] = false;

        auto size = components.size();
        for(auto i: components) {
            adjacency_list[i].price = size;
            adjacency_list[i].component = component_no;
        }
        component_no++;

        st->pop();
    }
}

// The function to do DFS traversal. It uses SCCUtil()
void dfs()
{
    int *disc = new int[N];
    int *low = new int[N];
    bool *stackMember = new bool[N];
    auto *st = new stack<int>();

    // Initialize disc and low, and stackMember arrays
    for (int i = 0; i < N; i++)
    {
        disc[i] = -1;
        low[i] = INT32_MAX;
        stackMember[i] = false;
    }

    // Call the recursive helper function to find strongly
    // connected components in DFS tree with vertex 'i'
    for (int i = 0; i < N; i++)
        if (disc[i] == -1)
            ssc(i, disc, low, st, stackMember);
}

int main() {
    fscanf(stdin, "%d %d", &N, &M);
    adjacency_list = vector<Node>(N);

    for (auto i = 0; i < M; i++) {
        if(i < N) {
            adjacency_list[i].idx = i;
        }

        int from, to;
        fscanf(stdin, "%d %d", &from, &to);

        adjacency_list[from].adjacent.push_back(to);
    }

    dfs();
    auto result = find_path();
    cout << result.price << " " << result.lenght << endl;

    return 0;
}
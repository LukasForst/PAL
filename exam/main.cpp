#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>
#include <climits>
#include <set>
#include <queue>
#include <algorithm>

#define DEBUG true
using namespace std;

int number_of_cities, number_of_highways, capital_city_label;

struct Highway {
    int src_label;
    int dest_label;

    int cost;
};

struct City {
    int label;
    int index = INT_MAX;

    list<Highway> highways;
};

vector<City> cities;

struct Subset {
    int parent, depth;
};

void label_cities() {
    queue<pair<int, int>> q;

    auto &highways_from_capital = cities[capital_city_label].highways;
    for (auto &high : highways_from_capital) {
        q.push(make_pair(0, high.dest_label));
    }

    while (!q.empty()) {
        auto pre_index_city_label = q.front();
        q.pop();
        auto new_index = pre_index_city_label.first + 1;
        auto &city = cities[pre_index_city_label.second];

        if (city.index > new_index) {
            city.index = new_index;

            for (auto &high : city.highways) {
                q.push(make_pair(city.index, high.dest_label));
            }
        }
    }
}

int find_step(vector<Subset> &subs, const int u) {
    auto &sub = subs[u];
    if (sub.parent != u) {
        sub.parent = find_step(subs, sub.parent);
    }

    return sub.parent;
}

vector<Subset> init_subsets() {
    vector<Subset> subs(number_of_cities);
    for (auto v = 0; v < number_of_cities; v++) {
        subs[v].parent = v;
        subs[v].depth = 0;
    }
    return subs;
}

void union_step(vector<Subset> &subs, const int u, const int v) {
    const auto r_u = find_step(subs, u);
    const auto r_v = find_step(subs, v);

    if (subs[r_u].depth < subs[r_v].depth) {
        subs[r_u].parent = r_v;
    } else if (subs[r_u].depth > subs[r_v].depth) {
        subs[r_v].parent = r_u;
    } else {
        subs[r_v].parent = r_u;
        subs[r_u].depth++;
    }
}

struct Edge {
    int src, dest, cost, index;
};

int kruskal(const vector<Edge> &edges) {
    int cost = 0;
    auto subs = init_subsets();

    for (const auto &e : edges) {
        const auto u_component = find_step(subs, e.src);
        const auto v_component = find_step(subs, e.dest);

        if (u_component == v_component)
            continue;

        union_step(subs, u_component, v_component);
        cost += e.cost;
    }

    return cost;
}

int main() {
    cin >> number_of_cities >> number_of_highways >> capital_city_label;
    cities.resize(number_of_cities);

    for (auto i = 0; i < number_of_highways; i++) {
        if (i < number_of_cities) {
            cities[i].label = i;
        }

        int src, dest, cost;
        fscanf(stdin, "%d %d %d", &src, &dest, &cost);

        cities[src].highways.push_back(Highway{src, dest, cost});
        cities[dest].highways.push_back(Highway{dest, src, cost});
    }

    cities[capital_city_label].index = 0;
    label_cities();

    list<Edge> edges;

    for (auto &city : cities) {
        for (auto &highway : city.highways) {
            Edge e{city.label, highway.dest_label, highway.cost, max(city.index, cities[highway.dest_label].index)};
            edges.push_back(e);
        }
    }

    edges.sort([](const Edge &e1, const Edge &e2) {
        return e1.index != e2.index ? e1.index <= e2.index : e1.cost <= e2.cost;
    });

    vector<Edge> vector_edges(edges.begin(), edges.end());

    cout << kruskal(vector_edges) << endl;
    return 0;
}

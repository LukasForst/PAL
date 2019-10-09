#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <cstdlib>
#include <set>
#include <algorithm>

using namespace std;

int R, C, P, K, V;

struct Vertex
{
    int id;
    int potential;
    int r;
    int c;

    int nearest_nzp_vertex_idx;
    int distance_to_nzp_vertex;

    bool is_nzpv;

    list<int> adjencent;
};

struct Distance
{
    int neighbor_idx;
    int weight;
};

struct Subset
{
    int parent, depth;
};

void fill_nearest(const int r, const int c, list<int> &adjencent)
{
    if (r + 1 != R)
    {
        adjencent.push_back((r + 1) * C + c);
    }

    if (r - 1 != -1)
    {
        adjencent.push_back((r - 1) * C + c);
    }

    if (c + 1 != C)
    {
        adjencent.push_back(r * C + (c + 1));
    }

    if (c - 1 != -1)
    {
        adjencent.push_back(r * C + (c - 1));
    }
}

void fill_mesh(vector<Vertex> &data)
{
    for (int r = 0; r < R; r++)
    {
        for (int c = 0; c < C; c++)
        {
            const auto id = r * C + c;
            Vertex &v = data[id];
            v.id = id;
            v.r = r;
            v.c = c;
            v.potential = 0;
            v.distance_to_nzp_vertex = INT32_MAX - 2;
            v.is_nzpv = false;
            fill_nearest(r, c, v.adjencent);
        }
    }
}

vector<int> nzp_vertices(vector<Vertex> &data)
{
    vector<int> nzps(P);
    for (int i = 0; i < P; i++)
    {
        int r, c, p;
        fscanf(stdin, "%d %d %d", &r, &c, &p);
        const auto id = (r - 1) * C + (c - 1);
        auto &v = data[id];
        v.is_nzpv = true;
        v.distance_to_nzp_vertex = 0;
        v.nearest_nzp_vertex_idx = id;
        v.potential = p;

        nzps[i] = id;
    }
    return nzps;
}

void extra_edges(vector<Vertex> &data)
{
    for (auto i = 0; i < K; i++)
    {
        int r1, c1, r2, c2;
        fscanf(stdin, "%d %d %d %d", &r1, &c1, &r2, &c2);

        const auto id1 = (r1 - 1) * C + (c1 - 1);
        const auto id2 = (r2 - 1) * C + (c2 - 1);

        data[id1].adjencent.push_back(id2);
        data[id2].adjencent.push_back(id1);
    }
}

void change_potential(Vertex &to, Vertex &from, queue<int> &q)
{
    if (to.is_nzpv)
    {
        return;
    }

    to.distance_to_nzp_vertex = from.distance_to_nzp_vertex + 1;
    to.nearest_nzp_vertex_idx = from.nearest_nzp_vertex_idx;
    to.potential = from.potential;
    q.emplace(to.id);
}

void explore_vertex(const int idx, queue<int> &q, vector<Vertex> &data)
{
    auto &v = data[idx];
    for (int adj_idx : v.adjencent)
    {
        auto &adj = data[adj_idx];

        if (adj.distance_to_nzp_vertex > v.distance_to_nzp_vertex + 1)
        {
            change_potential(adj, v, q);
        }
        else if (adj.distance_to_nzp_vertex == v.distance_to_nzp_vertex + 1)
        {
            if (data[adj.nearest_nzp_vertex_idx].potential > data[v.nearest_nzp_vertex_idx].potential)
            {
                change_potential(adj, v, q);
            }
        }
    }
}

void set_nzpv(const vector<int> &nzps, vector<Vertex> &data)
{
    queue<int> q;
    for (const auto nzp_idx : nzps)
    {
        q.emplace(nzp_idx);
    }

    while (!q.empty())
    {
        const auto to_explore_idx = q.front();
        q.pop();
        explore_vertex(to_explore_idx, q, data);
    }
}

struct Edge
{
    int src, dest, weight;
};

void create_weighted_edges(list<Edge> &edges, const vector<Vertex> &data)
{
    auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) {
        if (a.first == b.first)
            return a.second < b.second;
        else
            return a.first < b.first;
    };

    set<pair<int, int>, decltype(cmp)> inserted(cmp);

    for (int u_idx = 0; u_idx < V; u_idx++)
    {
        auto &u = data[u_idx];
        for (auto &v_idx : u.adjencent)
        {
            pair<int, int> p{min(u_idx, v_idx), max(u_idx, v_idx)};
            if (!inserted.insert(p).second)
            {
                continue;
            }

            auto &v = data[v_idx];
            Edge e{u_idx, v_idx, 0};
            e.weight = u.distance_to_nzp_vertex + v.distance_to_nzp_vertex;
            e.weight += abs(data[u.nearest_nzp_vertex_idx].potential - data[v.nearest_nzp_vertex_idx].potential);
            edges.push_back(e);
        }
    }
}

int track(const list<Edge> &edges)
{
    auto w = 0;
    for (const auto &e : edges)
    {
        w += e.weight;
    }
    return w;
}

vector<Subset> init_subsets()
{
    vector<Subset> subs(V);
    for (auto v = 0; v < V; v++)
    {
        subs[v].parent = v;
        subs[v].depth = 0;
    }
    return subs;
}

int find_step(vector<Subset> &subs, const int u)
{
    auto &sub = subs[u];
    if (sub.parent != u)
    {
        sub.parent = find_step(subs, sub.parent);
    }

    return sub.parent;
}

void union_step(vector<Subset> &subs, const int u, const int v)
{
    const auto r_u = find_step(subs, u);
    const auto r_v = find_step(subs, v);

    if (subs[r_u].depth < subs[r_v].depth)
    {
        subs[r_u].parent = r_v;
    }
    else if (subs[r_u].depth > subs[r_v].depth)
    {
        subs[r_v].parent = r_u;
    }
    else
    {
        subs[r_v].parent = r_u;
        subs[r_u].depth++;
    }
}

int kruskal(const vector<Edge> &edges)
{
    const auto final_edges_count = V - 1;

    list<Edge> final_tree;
    auto subs = init_subsets();
    for (const auto &e : edges)
    {
        const auto u_component = find_step(subs, e.src);
        const auto v_component = find_step(subs, e.dest);
        
        if (u_component == v_component)
            continue;

        final_tree.push_back(e);

        if ((int) final_tree.size() == final_edges_count)
            break;
        
        union_step(subs, u_component, v_component);
    }

    return track(final_tree);
}

int main()
{
    fscanf(stdin, "%d %d %d %d", &R, &C, &P, &K);
    V = R * C;

    vector<Vertex> vertices(V);
    fill_mesh(vertices);
    const auto &nzps = nzp_vertices(vertices);
    extra_edges(vertices);

    set_nzpv(nzps, vertices);
    list<Edge> edges;
    create_weighted_edges(edges, vertices);

    edges.sort([](const Edge &e1, const Edge &e2) { return e1.weight <= e2.weight; });
    vector<Edge> vector_edges(edges.begin(), edges.end());

    cout << kruskal(vector_edges) << "\n";
    return 0;
}
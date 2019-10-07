#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>

using namespace std;

int R, C, P, K;

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

void fill_nearest(int r, int c, list<int> &adjencent)
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
            int id = r * C + c;
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
        int id = (r - 1) * C + (c - 1);
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
    for (int i = 0; i < K; i++)
    {
        int r1, c1, r2, c2;
        fscanf(stdin, "%d %d %d %d", &r1, &c1, &r2, &c2);

        int id1 = (r1 - 1) * C + (c1 - 1);
        int id2 = (r2 - 1) * C + (c2 - 1);

        data[id1].adjencent.push_back(id2);
        data[id2].adjencent.push_back(id1);
    }
}

void print_vertex(int number, vector<Vertex> &vertices)
{
    int idx = number - 1;
    auto &v = vertices[idx];
    cout << "Vertex No: " << idx + 1;
    cout << " distance: " << v.distance_to_nzp_vertex;
    cout << " potential: " << v.potential;
    cout << " nzpv: " << v.nearest_nzp_vertex_idx + 1 << "\n";
    for (auto &adj : v.adjencent)
    {
        cout << adj + 1 << "\n";
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

void explore_vertex(int idx, queue<int> &q, vector<Vertex> &data)
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

void set_nzpv(vector<int> &nzps, vector<Vertex> &data)
{
    queue<int> q;
    for (int nzp_idx : nzps)
    {
        q.emplace(nzp_idx);
    }

    while (!q.empty())
    {
        int to_explore_idx = q.front();
        q.pop();
        explore_vertex(to_explore_idx, q, data);
    }
}

int run()
{
    fscanf(stdin, "%d %d %d %d", &R, &C, &P, &K);

    vector<Vertex> vertices(R * C);
    fill_mesh(vertices);
    vector<int> nzps = nzp_vertices(vertices);
    extra_edges(vertices);

    set_nzpv(nzps, vertices);

    print_vertex(5, vertices);
    return 0;
}

int main()
{
    return run();
}
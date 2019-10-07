#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;

struct Vertex
{
    int id;
    int potential;
    int r;
    int c;
    Vertex *nearest_nzp_vertex;
    int distance_to_nzp_vertex;

    list<int> adjencent;
};

void fill_nearest(int r, int c, int R, int C, list<int> &adjencent)
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

    if (c - 1 !=  -1)
    {
        adjencent.push_back(r * C + (c - 1));
    }
}

void fill_mesh(int R, int C, vector<Vertex> &data)
{
    for (int r = 0; r < R; r++)
    {
        for (int c = 0; c < C; c++)
        {
            int id = r * C + c;
            Vertex *v = &data[id];
            v->id = id;
            v->r = r;
            v->c = c;
            v->potential = 0;
            fill_nearest(r, c, R, C, v->adjencent);
        }
    }
}

void nzp_vertices(int P, int C, vector<Vertex> &data)
{
    for (int i = 0; i < P; i++)
    {
        int r, c, p;
        fscanf(stdin, "%d %d %d", &r, &c, &p);
        int id = (r - 1) * C + (c - 1);

        data[id].potential = p;
    }
}

void extra_edges(int K, int C, vector<Vertex> &data)
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


int run()
{
    int R, C, P, K;
    fscanf(stdin, "%d %d %d %d", &R, &C, &P, &K);

    vector<Vertex> vertices(R * C);
    fill_mesh(R, C, vertices);
    nzp_vertices(P, C, vertices);
    extra_edges(K, C, vertices);

    int idx = 5;
    cout << "pot: " << vertices[idx].potential << "\n";
    for (auto &adj: vertices[idx].adjencent) {
        cout << adj + 1 << "\n";
    }

    return 0;
}

int main()
{
    return run();
}
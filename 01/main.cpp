#include <iostream>
#include <list>
#include <vector>

using namespace std;

struct Vertex
{
    int id;
    int potential;
    int r;
    int c;
    Vertex *parent;
};

bool isZeroPotential(Vertex &v)
{
    return v.potential == 0;
}

void fillMesh(int R, int C, vector<Vertex> &data)
{
    for(int r = 0; r < R; r++)
    {
        for(int c = 0; c < C; c++){
            int id = r*C+c;

            data[id].id = id;
            data[id].r = r;
            data[id].c = c;
            data[id].potential = 0;
        }
    }
    return;
}

void nonZeroPotentialVertexes(int P, int C, vector<Vertex> &data)
{
    for (int i = 0; i < P; i++)
    {
        int r, c, p;
        fscanf(stdin, "%d %d %d", &r, &c, &p);
        int id = r*C+c;
        data[id].id = id;
        data[id].r = r;
        data[id].c = c;
        data[id].potential = p;
    }
    return;
}

int run()
{
    int R, C, P, K;
    fscanf(stdin, "%d %d %d %d", &R, &C, &P, &K);
    
    vector<Vertex> vertices(R * C);
    fillMesh(R, C, vertices);
    nonZeroPotentialVertexes(P, C, vertices);



    return 0;
}

int main() {
    vector<Vertex> v(2);
    cout << v[0].potential;
    return 0;
}
#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <cstdlib>
#include <set>
#include <algorithm>

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

struct Distance
{
    int neighbor_idx;
    int weight;
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

void print_vertex(const int number, const vector<list<Distance>> &adjency_list, const vector<Vertex> &vertices)
{
    int idx = number - 1;
    auto &v = vertices[idx];
    cout << "Vertex No: " << idx + 1;
    cout << " distance: " << v.distance_to_nzp_vertex;
    cout << " potential: " << v.potential;
    cout << " nzpv: " << v.nearest_nzp_vertex_idx + 1 << "\n";
    cout << "Adjent:\n";

    for (auto &adj : adjency_list[idx])
    {
        cout << "idx: " << adj.neighbor_idx + 1 << " weight: " << adj.weight << "\n";
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

struct Edge {
    int src, dest, weight;
};

void create_weighted_edges(list<Edge> &edges, const vector<Vertex> &data)
{
    int size = R * C;

    auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) 
    { 
        if(a.first == b.first) return a.second < b.second;
        else return a.first < b.first;
    };

    set<pair<int, int>, decltype(cmp)> inserted(cmp);

    for (int u_idx = 0; u_idx < size; u_idx++)
    {
        auto &u = data[u_idx];
        for (auto &v_idx : u.adjencent)
        {
            pair<int, int> p { min(u_idx, v_idx), max(u_idx, v_idx) };
            if(!inserted.insert(p).second) 
            {
                continue;
            }

            auto &v = data[v_idx];
            Edge e { u_idx, v_idx, 0 };
            e.weight = u.distance_to_nzp_vertex + v.distance_to_nzp_vertex;
            e.weight += abs(data[u.nearest_nzp_vertex_idx].potential - data[v.nearest_nzp_vertex_idx].potential);
            edges.push_back(e);
        }
    }
}

// A structure to represent a subset for union-find  
class subset  
{  
    public: 
    int parent;  
    int rank;  
};  
  
// A utility function to find set of an element i  
// (uses path compression technique)  
int find(subset subsets[], int i)  
{  
    // find root and make root as parent of i  
    // (path compression)  
    if (subsets[i].parent != i)  
        subsets[i].parent = find(subsets, subsets[i].parent);  
  
    return subsets[i].parent;  
}  
  
// A function that does union of two sets of x and y  
// (uses union by rank)  
void Union(subset subsets[], int x, int y)  
{  
    int xroot = find(subsets, x);  
    int yroot = find(subsets, y);  
  
    // Attach smaller rank tree under root of high  
    // rank tree (Union by Rank)  
    if (subsets[xroot].rank < subsets[yroot].rank)  
        subsets[xroot].parent = yroot;  
    else if (subsets[xroot].rank > subsets[yroot].rank)  
        subsets[yroot].parent = xroot;  
  
    // If ranks are same, then make one as root and  
    // increment its rank by one  
    else
    {  
        subsets[yroot].parent = xroot;  
        subsets[xroot].rank++;  
    }  
}  
  
// The main function to construct MST using Kruskal's algorithm  
void KruskalMST(const list<Edge> &edges_data)  
{  
    vector<Edge> edges(edges_data.begin(), edges_data.end());

    int E = edges.size;
    int V = R*C;
    Edge result[V]; // Tnis will store the resultant MST  
    int e = 0; // An index variable, used for result[]  
    int i = 0; // An index variable, used for sorted edges  

    // Allocate memory for creating V ssubsets  
    subset *subsets = new subset[( V * sizeof(subset) )];  
  
    // Create V subsets with single elements  
    for (int v = 0; v < V; ++v)  
    {  
        subsets[v].parent = v;  
        subsets[v].rank = 0;  
    }  
  
    // Number of edges to be taken is equal to V-1  
    while (e < V - 1 && i < E)  
    {  
        // Step 2: Pick the smallest edge. And increment  
        // the index for next iteration  
        Edge next_edge = edges[i++];  
  
        int x = find(subsets, next_edge.src);  
        int y = find(subsets, next_edge.dest);  
  
        // If including this edge does't cause cycle,  
        // include it in result and increment the index  
        // of result for next edge  
        if (x != y)  
        {  
            result[e++] = next_edge;  
            Union(subsets, x, y);  
        }  
        // Else discard the next_edge  
    }  
  
    // print the contents of result[] to display the  
    // built MST  
    cout<<"Following are the edges in the constructed MST\n";  
    for (i = 0; i < e; ++i)  
        cout<<result[i].src<<" -- "<<result[i].dest<<" == "<<result[i].weight<<endl;  
    return;  
}  


int run()
{
    fscanf(stdin, "%d %d %d %d", &R, &C, &P, &K);

    vector<Vertex> vertices(R * C);
    fill_mesh(vertices);
    const auto &nzps = nzp_vertices(vertices);
    extra_edges(vertices);

    set_nzpv(nzps, vertices);
    list<Edge> edges;
    create_weighted_edges(edges, vertices);

    // for(const auto &e : edges) {
    //     cout << e.src + 1 << ":" << e.dest + 1 << " - " << e.weight << "\n";
    // }
    cout << "sorted:\n";
    edges.sort([](const Edge &e1, const Edge &e2){ return e1.weight <= e2.weight; });

    KruskalMST(edges);

    // for(const auto &e : edges) {
    //     cout << e.src + 1 << ":" << e.dest + 1 << " - " << e.weight << "\n";
    // }


    return 0;
}

int main()
{
    return run();
}
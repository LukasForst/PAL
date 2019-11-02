#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <cstdlib>
#include <set>
#include <algorithm>

using namespace std;

int N, M;

int main() {
    fscanf(stdin, "%d %d", &N, &M);
    vector<list<int>> adjency_list(N);

    for (auto i = 0; i < M; i++) {
        int from, to;
        fscanf(stdin, "%d %d", &from, &to);
        adjency_list[from].push_back(to);
    }

    return 0;
}
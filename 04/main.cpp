#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>
#include <limits>
#include <set>

#define DEBUG 0
using namespace std;

int max_deletions;

string dna_sequence;

struct Automata {
    string sequence = "";
    int cost = 0;
};

vector<Automata> automatas;

struct Result {
    string maximal_sequence = "";
    int cost = 0;
    int deletions = 0;
};


Result use_automat(int idx, const Automata &automata) {
    Result r = Result();

    for (const auto &c : automata.sequence) {
        if (idx < dna_sequence.length() && c == dna_sequence[idx]) {
            idx++;
            r.maximal_sequence += c;
        } else {
            if (r.deletions >= max_deletions) break;
            r.deletions++;
        }
    }
    r.cost = automata.cost + r.deletions;
    return r;
}

struct NextIter {
    int idx;
    int cost;
    double value;
    string str;

    bool operator<(const NextIter &iter) const {
        return value < iter.value;
    }
};

int best_cost = numeric_limits<int>::max();
int best_basis_units_count = 0;

void run(int idx, int cost, int basis_unit_count) {
    if (cost > best_cost || (cost == best_cost && best_basis_units_count <= basis_unit_count)) {
        return;
    } else if (idx == dna_sequence.length()) {
        if (cost < best_cost) {
            best_cost = cost;
            best_basis_units_count = basis_unit_count;
        } else if (cost == best_cost && best_basis_units_count > basis_unit_count) {
            best_basis_units_count = basis_unit_count;
        }
        return;
    }

    list<NextIter> iters;
    for (const auto &automata: automatas) {
        auto r = use_automat(idx, automata);

        int next_idx = idx + (int) r.maximal_sequence.length();
        if (next_idx == idx) continue;

        int next_cost = cost + r.cost;

        iters.push_back(NextIter{next_idx, next_cost, (double) next_cost / (double) next_idx, r.maximal_sequence});
    }

    basis_unit_count++;

    iters.sort();
    set<string> visited;
    for (auto &i : iters) {
        if (visited.find(i.str) == visited.end()) {
            visited.insert(i.str);
            run(i.idx, i.cost, basis_unit_count);
        }
    }
}

int main() {
    cin >> dna_sequence;

    int basis_unit_types;
    cin >> basis_unit_types;
    cin >> max_deletions;

    automatas.resize(basis_unit_types);

    for (auto &base : automatas) {
        cin >> base.cost;
        cin >> base.sequence;
    }

    run(0, 0, 0);
    cout << best_cost << " " << best_basis_units_count << endl;
}
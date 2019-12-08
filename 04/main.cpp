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
            if (r.deletions == max_deletions) {
                return Result();
            }
            r.deletions++;
        }
    }
    r.cost = automata.cost + r.deletions;
    return r;
}

struct Reach {
    int idx;
    int cost;
};

list<Reach> get_reachable_indicies(int idx, const Automata &dfa) {
    list<Reach> indicies{};
    auto r = use_automat(idx, dfa);

    if (r.maximal_sequence.empty()) return indicies;

    indicies.push_back(Reach{idx + (int) r.maximal_sequence.length(), r.cost});

    int deletions_performed = 1;
    for (int i = r.deletions; i < max_deletions; i++) {
        auto next_idx = idx + (int) r.maximal_sequence.length() - deletions_performed;
        if (next_idx < 1) break;

        indicies.push_back(Reach{next_idx, r.cost + deletions_performed});
        deletions_performed++;
    }

    return indicies;
}


struct Index {
    int cost = numeric_limits<int>::max();
    int basis_count = numeric_limits<int>::max();
};

void run() {
    vector<Index> indicies(dna_sequence.length() + 1);
    indicies[0].cost = 0;
    indicies[0].basis_count = 0;

    for (int idx = 0; idx < (int) dna_sequence.length(); idx++) {
        auto &idx_value = indicies[idx];

        for (const auto &dfa : automatas) {
            auto reach = get_reachable_indicies(idx, dfa);
            for (auto &r : reach) {
                auto &reachable_index = indicies[r.idx];

                auto index_cost = idx_value.cost + r.cost;
                auto index_basis_count = idx_value.basis_count + 1;

                if (index_cost < 0) {
                    continue;
                }

                if (reachable_index.cost > index_cost) {
                    reachable_index.cost = index_cost;
                    reachable_index.basis_count = index_basis_count;
                } else if (reachable_index.cost == index_cost && reachable_index.basis_count > index_basis_count) {
                    reachable_index.basis_count = index_basis_count;
                }
            }
        }
    }

    if (DEBUG) {
        for (const auto &a : indicies) {
            cout << a.cost << endl;
        }
    }

    auto &last = indicies.back();
    cout << last.cost << " " << last.basis_count << endl;
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

    run();
}
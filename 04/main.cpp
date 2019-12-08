#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>
#include <limits>

#define DEBUG 0
using namespace std;

int max_deletions;

string dna_sequence;

struct Automata {
    string sequence;
    int length;
    int cost;
};

vector<Automata> automatas;

struct Result {
    string maximal_sequence = "";
    int cost = 0;
    int deletions = 0;
    double value = numeric_limits<double>::infinity() - 1;
};


Result use_automat(int idx, const Automata &automata) {
    Result r = Result();

    for (const auto &c : automata.sequence) {
        if (r.deletions > max_deletions) break;

        if (c == dna_sequence[idx]) {
            idx++;
            r.maximal_sequence += c;
        } else {
            r.deletions++;
        }
    }
    r.cost = automata.cost + r.deletions;

    if (r.maximal_sequence.length() != 0) {
        r.value = ((double) r.cost) / ((double) r.maximal_sequence.length());
    } else {
        r.value = numeric_limits<double>::infinity();
    }

    return r;
}

int best_cost = numeric_limits<int>::max();
int best_basis_units_count = 0;

void run(int idx, int cost, int basis_unit_count) {
    if (cost >= best_cost) {
        return;
    } else if (idx == dna_sequence.length()) {
        if (cost < best_cost) {
            best_cost = cost;
            best_basis_units_count = basis_unit_count;
        }
        return;
    }

    for (const auto &automata: automatas) {
        auto r = use_automat(idx, automata);

        int next_idx = idx + (int) r.maximal_sequence.length();
        if (next_idx == idx) continue;

        int next_cost = cost + r.cost;
        int next_basis_count = basis_unit_count + 1;

        run(next_idx, next_cost, next_basis_count);

        auto deletions = r.deletions;
        while (deletions < max_deletions) {
            deletions++;
            next_idx--;
            next_cost++;

            run(next_idx, next_cost, next_basis_count);
        }
    }

}

//void run() {
//    int idx = 0;
//    int last_idx = dna_sequence.length();
//
//    int cost = 0;
//    int basis_unit_count = 0;
//
//    while (idx != last_idx) {
//        Result best = Result();
//
//        for (const auto &automata: automatas) {
//
//            auto r = use_automat(idx, automata);
//            if (r.value < best.value) {
//                best = r;
//            }
//        }
//
//        basis_unit_count++;
//        cost += best.cost;
//        idx += best.maximal_sequence.length();
//        if (DEBUG) {
//            cout << best.maximal_sequence << endl;
//        }
//    }
//
//    cout << cost << " " << basis_unit_count << endl;
//}

int main() {
    cin >> dna_sequence;

    int basis_unit_types;
    cin >> basis_unit_types;
    cin >> max_deletions;

    automatas.resize(basis_unit_types);

    for (auto &base : automatas) {
        cin >> base.cost;
        cin >> base.sequence;
        base.length = base.sequence.length();
        if (DEBUG) cout << "Seq: " << base.sequence << " - " << base.cost << endl;
    }

    run(0, 0, 0);
    cout << best_cost << " " << best_basis_units_count << endl;
}
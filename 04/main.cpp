#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>
#include <limits>

#define DEBUG 1

using namespace std;

int max_deletions;

string dna_sequence;

struct Automata {
    string sequence;
    int length;
    int price;
};

vector<Automata> automatas(1);

struct Result {
    string maximal_sequence = "";
    int price = 0;
    double value = numeric_limits<double>::infinity() - 1;
};


Result use_automat(int idx, const Automata &automata) {
    Result r = Result();

    int deletions = 0;

    for (const auto &c : automata.sequence) {
        if (deletions > max_deletions) break;

        if (c == dna_sequence[idx]) {
            idx++;
            r.maximal_sequence += c;
        } else {
            deletions++;
        }
    }
    r.price = automata.price + deletions;

    if (r.maximal_sequence.length() != 0) {
        r.value = ((double) r.price) / ((double) r.maximal_sequence.length());
    } else {
        r.value = numeric_limits<double>::infinity();
    }

    return r;
}

void run() {
    int idx = 0;
    int last_idx = dna_sequence.length();

    int cost = 0;
    int basis_unit_count = 0;

    while (idx != last_idx) {
        Result best = Result();

        for (const auto &automata: automatas) {

            auto r = use_automat(idx, automata);
            if (r.value < best.value) {
                best = r;
            }
        }

        basis_unit_count++;
        cost += best.price;
        idx += best.maximal_sequence.length();
        if (DEBUG) {
            cout << best.maximal_sequence << endl;
        }
    }

    cout << cost << " " << basis_unit_count << endl;
}

int main() {
    cin >> dna_sequence;

    int basis_unit_types;
    cin >> basis_unit_types;
    cin >> max_deletions;

    automatas.resize(basis_unit_types);

    for (auto &base : automatas) {
        cin >> base.price;
        cin >> base.sequence;
        base.length = base.sequence.length();
        if (DEBUG) cout << "Seq: " << base.sequence << " - " << base.price << endl;
    }

    run();
}
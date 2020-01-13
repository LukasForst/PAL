#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

string configuration;
int max_changes;

int max_min_one =  numeric_limits<int>::max() - 1;

struct MinMaxSolution {
    MinMaxSolution() {
        min = max_min_one;
        max = numeric_limits<int>::min();
    }

    MinMaxSolution(int m, int mx) {
        min = m;
        max = mx;
    }

    int min;
    int max;
    bool done = false;


    void update(const MinMaxSolution &other) {
        min = std::min(min, other.min + 1);
        max = std::max(max, other.max + 1);
    }
};

int* leven(const string &s2) {
    int n = s2.length() + 1;

    auto costs = new int[n];

    for (auto k = 0; k < n; k++) {
        costs[k] = k;
    }

    auto i = 0;
    for (const auto &w1: configuration) {
        costs[0] = i + 1;

        auto corner = i;
        auto j = 0;
        for (const auto &w2: s2) {
            auto upper = costs[j + 1];
            if (w2 == w1) {
                costs[j + 1] = corner;
            } else {
                costs[j + 1] = MIN3(costs[j] + 1, upper + 1, corner + 1);
            }
            corner = upper;
            j++;
        }
        i++;
    }

    return costs;
}

MinMaxSolution solve(const string &input, const string &config, const int max_len) {
    int input_len = input.length();
    MinMaxSolution solutions[input_len];

    for (auto current = input_len - 1; current >= 0; current--) {
        auto substring = input.substr(current, max_len);
        auto last_row = leven(substring);

        int sub_len = substring.length();
        for (auto i = 1; i <= sub_len; i++) {
            int wagon_len = min(sub_len, i);
        
            auto previous = current + wagon_len;

            if(last_row[wagon_len] > max_changes) continue;

            auto &prev = solutions[previous];
            auto &curr = solutions[current];
                
            if (previous == input_len) {
                if (curr.max == numeric_limits<int>::min()) {
                    curr.max = 1;
                }
                curr.min = min(curr.min, 1);

            } else if (prev.min != max_min_one) {
                curr.update(prev);
            }
        }
    }

    return solutions[0];
}

int main() {
    string input;
    cin >> input >> configuration >> max_changes;

    auto solution = solve(input, configuration, configuration.length() + max_changes);
    cout << solution.min << " " << solution.max << endl;

    return 0;
}

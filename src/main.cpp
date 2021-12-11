#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <set>

#include "utils.hpp"
#include "coordinates.hpp"

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;


struct Instruction {
    string name;
    int value;
};

vector<Instruction> read_instructions(const string& file_name) {
    auto lines = read_lines(file_name);

    vector<Instruction> instructions;

    for (auto line : lines) {
        auto words = split(line);
        instructions.push_back(Instruction{words[0], parse_int(words[1])});
    }
    return instructions;
}

vector<bool> text_to_bit_sequence(const string& text) {
    vector<bool> bit_sequence;
    for (auto c : text) {
        if (c == '1') {
            bit_sequence.push_back(true);
        } else if (c == '0') {
            bit_sequence.push_back(false);
        } else {
            throw domain_error("Unknown character" + c);
        }
    }
    return bit_sequence;
}

int bit_sequence_to_decimal_number(const vector<bool>& sequence) {
    double result = 0;
    for (int i = 0; i < sequence.size(); i ++) {
        if (sequence[i]) {result += pow(2., sequence.size() - i - 1);}
    }
    return (int) result;
}

void test_bit_sequence_to_decimal_number() {
    assert(bit_sequence_to_decimal_number(vector<bool>{true, true, true}) == 7);
    assert(bit_sequence_to_decimal_number(vector<bool>{true, false, true}) == 5);
    assert(bit_sequence_to_decimal_number(vector<bool>{false, true, false, true}) == 5);
    cout << "test_bit_sequence_to_decimal_number passed" << endl;
}

void day_01() {
    vector<int> input_numbers = read_lines_as_integers("inputs/day-01/input.txt");
    vector<int> measurement_data;

    for (int i = 0; i < input_numbers.size(); i ++) {
        if (i >= 2) {
            measurement_data.push_back(input_numbers[i] + input_numbers[i - 1] + input_numbers[i - 2]);
        }
    }

    int previous_measurement = INT32_MAX;
    int n_occurrences = 0;

    for (auto measurement : measurement_data) {
        if (previous_measurement < measurement) {
            n_occurrences += 1;
        }
        previous_measurement = measurement;
    }
    cout << n_occurrences << endl;
}

void day_02() {
    auto instructions = read_instructions("inputs/day-02/input.txt");

    int horizontal_position = 0;
    int depth = 0;
    int aim = 0;

    for (auto ins : instructions) {

        if (ins.name == "forward") {
            horizontal_position += ins.value;
            depth += ins.value * aim;
        } else if (ins.name == "down") {
            aim += ins.value;
        } else if (ins.name == "up") {
            aim -= ins.value;
        }
    }
    cout << horizontal_position * depth << endl;
}

vector<vector<bool>> prune_diagnostic_report(const vector<vector<bool>>& report, int bit_index, bool keep_value) {
    vector<vector<bool>> new_report;

    for (auto bit_sequence : report) {
        if (bit_sequence[bit_index] == keep_value) {
            new_report.push_back(bit_sequence);
        }
    }
    return new_report;
}

vector<bool> extract_data_from_report(const vector<vector<bool>>& report, bool (*comparator)(int n_ones, int n_zeroes)) {
    vector<vector<bool>> diagnostic_report(report);
    int n_bits = diagnostic_report[0].size();

    for (int bit_index = 0; bit_index < n_bits; bit_index ++) {

        if (diagnostic_report.size() == 1) break;

        int n_ones = 0;
        int n_zeroes = 0;
        for (int report_index = 0; report_index < diagnostic_report.size(); report_index ++) {
            diagnostic_report[report_index][bit_index] ? n_ones += 1: n_zeroes += 1;
        }
        bool keep_value = comparator(n_ones, n_zeroes);
        diagnostic_report = prune_diagnostic_report(diagnostic_report, bit_index, keep_value);
    }
    assert(diagnostic_report.size() == 1);
    return diagnostic_report[0];
}

void day_03() {
    auto bit_lines = read_lines("inputs/day-03/input.txt");

    vector<vector<bool>> diagnostic_report;

    for (auto line : bit_lines) {
        diagnostic_report.push_back(text_to_bit_sequence(line));
    }

    int oxygen_rating = bit_sequence_to_decimal_number(
        extract_data_from_report(
            diagnostic_report, [](int n_ones, int n_zeroes) {return n_ones >= n_zeroes ? true : false;}));

    int co2_rating = bit_sequence_to_decimal_number(
        extract_data_from_report(
            diagnostic_report, [](int n_ones, int n_zeroes) {return n_ones < n_zeroes ? true : false;}));
    cout << oxygen_rating * co2_rating << endl;
}

typedef struct {
    vector<vector<int>> numbers;
    vector<vector<bool>> mask;

} BingoTable;

BingoTable create_bingo_table(const vector<string>& table_lines) {
    vector<vector<int>> table;
    vector<vector<bool>> mask;
    

    for (auto line : table_lines) {
        vector<int> numbers = numbers_from_line(line);
        vector<bool> empty = vector<bool>(numbers.size(), false);
        table.push_back(numbers);
        mask.push_back(empty);
    }
    auto bingo_table = BingoTable{table, mask};
    return bingo_table;
}

vector<BingoTable> extract_bingo_tables(const vector<string>& lines) {

    vector<BingoTable> bingo_tables;

    int line_index = 0;
    while ((line_index < lines.size()) and (lines[line_index] == "")) {

        int table_index = line_index + 1;
        vector<string> table_lines;
        while ((table_index < lines.size()) and (lines[table_index] != "")) {
            table_lines.push_back(lines[table_index]);
            table_index ++;
        }
        bingo_tables.push_back(create_bingo_table(table_lines));

        line_index = table_index;
    }
    return bingo_tables;
}

void print_table(const BingoTable& t) {
    cout << "TABLE" << endl;
    for (auto row : t.numbers) {
        for (auto col : row) {
            cout << col << " ";
        }
        cout << endl;
    }
    cout << endl;
    for (auto row : t.mask) {
        for (auto col : row) {
            cout << col << " ";
        }
        cout << endl;
    }
}


void burn_number(BingoTable& t, int number) {

    for (int r = 0; r < t.numbers.size(); r++) {
        for (int c = 0; c < t.numbers[r].size(); c++) {

            if (t.numbers[r][c] == number) {
                t.mask[r][c] = true;
            }
        }
    }
    // print_table(t);
}

bool has_row(const BingoTable& t) {
    for (int r = 0; r < t.numbers.size(); r++) {
        if (all_of(t.mask[r].cbegin(), t.mask[r].cend(), [](bool x){return x;})) {
            return true;
        }
    }
    return false;
}

bool has_column(const BingoTable& t) {
    
    for (int c = 0; c < t.numbers[0].size(); c++) {
        vector<bool> column_mask;
        for (int r = 0; r < t.numbers.size(); r++) {
            column_mask.push_back(t.mask[r][c]);
        }
        
        if (all_of(column_mask.cbegin(), column_mask.cend(), [](bool x){return x;})) {
            return true;
        }
    }
    return false;
}

bool has_won(const BingoTable& t) {
    return has_row(t) or has_column(t);
}

int unmarked_score(const BingoTable& t) {
    int result = 0;
    for (int r = 0; r < t.numbers.size(); r++) {
        for (int c = 0; c < t.numbers[r].size(); c++) {
            if (not t.mask[r][c]) result += t.numbers[r][c];
        }
    }
    return result;
}

void day_04() {
    auto input_lines = read_lines("inputs/day-04/input.txt");

    auto numbers = numbers_from_line(input_lines[0], ",");

    auto table_lines = vector<string>(input_lines.begin() + 1, input_lines.end());
    auto bingo_tables = extract_bingo_tables(table_lines);
    set<int> won_table_indices;

    for (int number : numbers) {

        for (int i = 0; i < bingo_tables.size(); i ++) {
            auto t = bingo_tables[i];
            burn_number(t, number);
            bingo_tables[i] = t;

            if (has_won(t)) {
                won_table_indices.insert(i);
                if (won_table_indices.size() == bingo_tables.size()) {
                    int score = unmarked_score(t);
                    print_table(t);
                    cout << score * number << endl;
                    cout << endl;
                }
            }
        }
    }
}

typedef struct {
    pair<int, int> source;
    pair<int, int> target;
    pair<int, int> direction;
    bool is_diagonal;
} Line;




CoordinatesLine create_line(const string& text_line) {
    auto words = split(text_line, "->");
    auto source_words = split(words[0], ",");
    auto target_words = split(words[1], ",");
    

    auto source = Coordinates(parse_int(source_words[0]), parse_int(source_words[1]));
    auto target = Coordinates(parse_int(target_words[0]), parse_int(target_words[1]));

    return CoordinatesLine(source, target);
}

vector<CoordinatesLine> extract_lines(const vector<string>& text_lines) {
    vector<CoordinatesLine> lines;
    for_each(text_lines.cbegin(), text_lines.cend(), [&](string x){lines.push_back(create_line(x));});
    return lines;
}

int draw_line(const CoordinatesLine& line, vector<vector<int>>& map) {
    // if (line.is_diagonal) return 0;
    int n_twos_update = 0;

    auto current = line.source();

    while (current != line.target()) {
        if (map[current.y()][current.x()] == 1) n_twos_update += 1;
        map[current.y()][current.x()] += 1;
        current = current + line.direction();
    }
    if (map[current.y()][current.x()] == 1) n_twos_update += 1;
    map[current.y()][current.x()] += 1;

    return n_twos_update;
}


void day_05() {
    auto input_lines = read_lines("inputs/day-05/input.txt");

    auto lines = extract_lines(input_lines);
    vector<vector<int>> map(1000, vector<int>(1000, 0));

    int n_twos = 0;
    
    for (auto line : lines) {
        n_twos += draw_line(line, map);
    }

    cout << n_twos << endl;
}

int main () {
    auto lines = read_lines("inputs/day-06/input.txt");
    auto fish_numbers = numbers_from_line(lines[0], ",");

    int n_days = 80;

    for (int day_number = 0; day_number < n_days; day_number ++) {
        auto n_fish = fish_numbers.size();
        for (int i = 0; i < n_fish; i ++) {
            auto fish_number = fish_numbers[i];
            if (fish_number == 0) {
                fish_numbers.push_back(8);
                fish_numbers[i] = 6;
            } else {
                fish_numbers[i] -= 1;
            }
        }
    }

    cout << fish_numbers.size() << endl;
}
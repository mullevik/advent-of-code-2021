#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <cassert>
#include <cmath>

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

using namespace std;

vector<string> split(const string& sentence, const string& delimiter) {
    vector<string> words;
    istringstream iss(sentence);

    size_t last_found_index = 0;
    
    while (last_found_index < sentence.size()) {

        int found_index = sentence.find(delimiter, last_found_index);

        if (found_index == string::npos) {
            words.push_back(sentence.substr(last_found_index, delimiter.size() - last_found_index));
            return words;
        } else {
            words.push_back(sentence.substr(last_found_index, found_index - last_found_index));
            last_found_index = found_index + delimiter.size();
        }
    }
    return words;
}

vector<string> split(const string& sentence) {
    return split(sentence, " ");
}


void test_split() {
    assert(split("Hello world").size() == 2);
    assert(split("single-word").size() == 1);
    assert(split("single-word", "-").size() == 2);
    assert(split("long text, with, commas,", ",").size() == 3);
    cout << "test_split passed" << endl;
}


vector<string> read_lines(const string& file_name) {
    ifstream fs(file_name);
    if (!fs.good()) {
        throw invalid_argument("File not found: " + file_name);
    }
    vector<string> input_lines;
    string line;
    while (getline(fs, line)) {
        input_lines.push_back(line);
    }
    return input_lines;
}

int parse_int(const string& s) {
    return stoi(s);
}

vector<int> read_lines_as_integers(const string& file_name) {
    vector<int> integers;
    for (auto line : read_lines(file_name)) {
        integers.push_back(parse_int(line));
    }
    return integers;
}

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

int main() { 
    
}
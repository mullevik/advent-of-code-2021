#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <cassert>
#include <cmath>
#include <algorithm>


int parse_int(const std::string& s) {
    return std::stoi(s);
}

std::vector<std::string> split(const std::string& sentence, const std::string& delimiter) {
    std::vector<std::string> words;
    std::istringstream iss(sentence);

    size_t last_found_index = 0;
    
    while (last_found_index < sentence.size()) {

        int found_index = sentence.find(delimiter, last_found_index);

        if (found_index == std::string::npos) {
            auto word = sentence.substr(last_found_index, delimiter.size() - last_found_index);
            if (word != "") words.push_back(word);
            return words;
        } else {
            auto word = sentence.substr(last_found_index, found_index - last_found_index);
            if (word != "") words.push_back(word);
            last_found_index = found_index + delimiter.size();
        }
    }
    return words;
}

std::vector<std::string> split(const std::string& sentence) {
    return split(sentence, " ");
}


void test_split() {
    assert(split("Hello world").size() == 2);
    assert(split("single-word").size() == 1);
    assert(split("single-word", "-").size() == 2);
    assert(split("long text, with, commas,", ",").size() == 3);
    assert(split("1  2    3 4").size() == 4);
    std::cout << "test_split passed" << std::endl;
}

std::vector<int> numbers_from_line(const std::string& line, const std::string& delimiter = " ") {
    std::vector<int> numbers;
    std::vector<std::string> words = split(line, delimiter);
    std::for_each(words.begin(), words.end(), [&](std::string x){numbers.push_back(parse_int(x));});
    return numbers;
}


std::vector<std::string> read_lines(const std::string& file_name) {
    std::ifstream fs(file_name);
    if (!fs.good()) {
        throw std::invalid_argument("File not found: " + file_name);
    }
    std::vector<std::string> input_lines;
    std::string line;
    while (std::getline(fs, line)) {
        input_lines.push_back(line);
    }
    return input_lines;
}

std::vector<int> read_lines_as_integers(const std::string& file_name) {
    std::vector<int> integers;
    for (auto line : read_lines(file_name)) {
        integers.push_back(parse_int(line));
    }
    return integers;
}
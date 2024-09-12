#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int hash_value_(std::string &wprefix) {
    int n = wprefix.length();
    int value = 0;

    if (n >= 3) {
        value = (static_cast<unsigned char>(wprefix[0]) * 31 * 31 +
                 static_cast<unsigned char>(wprefix[1]) * 31 +
                 static_cast<unsigned char>(wprefix[2]));
    }
    else if (n == 2) {
        value = (static_cast<unsigned char>(wprefix[0]) * 31 +
                 static_cast<unsigned char>(wprefix[1]));
    }
    else if (n == 1) {
        value = static_cast<unsigned char>(wprefix[0]);
    }
    return value % 27000;
}

std::unordered_map<int, std::vector<long>> store_hashes(const std::string& file_name) {
    std::unordered_map<int,std::vector<long>> hash_map;
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_name << std::endl;
        return hash_map;
    }
    std::string word;
    long line_number = 0;

    while (file >> word) {
        int hash_value = hash_value_(word);
        hash_map[hash_value].push_back(line_number);
        line_number += word.length() + 1;
        file.close();
        return hash_map;
    }
}

int main() {
    std::string word;

    std::cout << "Enter a word : ";
    std::cin >> word;

    int hash_value = hash_value_(word);

    std::cout << "The hash value of '" << word << "' is: " << hash_value << std::endl;

    return 0;
}

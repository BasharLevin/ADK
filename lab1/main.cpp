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
    }
    file.close();
    return hash_map;
}
void display(const std::string& text_filename, long position, int context_size = 30) {
    std::ifstream text_file(text_filename);

    if (!text_file.is_open()) {
        std::cerr << "Error: Could not open text file: " << text_filename << std::endl;
        return;
    }
    long start = std::max(0L, position - context_size);
    text_file.seekg(start, std::ios::beg);

    std::string context(context_size * 2 + 1, '\0');
    text_file.read(&context[0], context_size * 2 + 1);

    std::cout << "Context: ..." << context << "..." << std::endl;

    text_file.close();
}
void search_word_and_display(const std::unordered_map<int, std::vector<long>>& hash_map, const std::string& word, const std::string& text_filename) {
    int search_hash = hash_value_(const_cast<std::string&>(word));

    auto it = hash_map.find(search_hash);
    if (it != hash_map.end()) {
        std::vector<long> positions = it->second;

        int occurrence_count = positions.size();
        std::cout << "The word '" << word << "' occurs " << occurrence_count << " times." << std::endl;

        int count_to_show = std::min(25, occurrence_count);
        for (int i = 0; i < count_to_show; ++i) {
            std::cout << "Occurrence " << (i + 1) << ": ";
            display(text_filename, positions[i]);
        }

        if (occurrence_count > 25) {
            std::cout << "There are more occurrences. Would you like to see the rest? (y/n): ";
            char response;
            std::cin >> response;
            if (response == 'y' || response == 'Y') {
                for (int i = 25; i < occurrence_count; ++i) {
                    std::cout << "Occurrence " << (i + 1) << ": ";
                    display(text_filename, positions[i]);
                }
            }
        }

    } else {
        std::cout << "Word '" << word << "' not found in the text." << std::endl;
    }
}

int main() {
    std::string word;

    std::cout << "Enter a word : ";
    std::cin >> word;
    std::unordered_map<int, std::vector<long>> hash_map = store_hashes("index.txt");
    search_word_and_display(hash_map, word, "test.txt");

    return 0;
}

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <iomanip>
#include "tokenizer.c"


std::string to_lowercase(const std::string &str) {
    std::string result;
    result.resize(str.size());
    std::transform(str.begin(), str.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}


unsigned int hash_prefix(const std::string &prefix) {
    int len = prefix.length();
    unsigned int value = 0;

    if (len >= 3) {
        value = (prefix[0] * 31 * 31) + (prefix[1] * 31) + prefix[2];
    } else if (len == 2) {
        value = (prefix[0] * 31) + prefix[1];
    } else if (len == 1) {
        value = prefix[0];
    }

    return value % 27000;
}


void create_index(std::vector<int> &indices, std::ifstream &file) {
    std::string line;
    std::set<std::string> seen_prefixes;
    std::streampos position = 0;

    while (std::getline(file, line)) {
        auto first_space = line.find(' ');
        std::string prefix = to_lowercase(line.substr(0, std::min(first_space, size_t(3))));

        if (seen_prefixes.find(prefix) == seen_prefixes.end()) {
            int hash_value = hash_prefix(prefix);
            if (indices[hash_value] == -1) {
                indices[hash_value] = position;

            }
            seen_prefixes.insert(prefix);
        }
        position += line.length() + 1;
    }

    file.clear();
    file.seekg(0);
}



std::vector<int> find_word_positions(const std::vector<int> &indices, std::ifstream &index_file, const std::string &word) {
    std::vector<int> matches;

    std::string prefix = word.substr(0, std::min(word.size(), size_t(3)));
    int hash_val = hash_prefix(prefix);

    std::streampos start = indices[hash_val];

    std::streampos end = -1;
    if (hash_val + 1 < indices.size() && indices[hash_val + 1] != -1) {
        end = indices[hash_val + 1];
    }

    if (end == -1 || end <= start) {
        index_file.seekg(0, std::ios::end);
        end = index_file.tellg();
    }

    if (start == -1) {
        std::cout << "No valid range for the prefix: " << prefix << std::endl;
        return matches;
    }

    std::cout << "Searching for word: " << word << " with prefix: " << prefix << " in range [" << start << ", " << end << "]" << std::endl;

    std::string line;
    while (end - start > 1000) {
        std::streampos mid = (start + end) / 2;
        index_file.seekg(mid);

        std::getline(index_file, line);

        if (!std::getline(index_file, line)) {
            break;
        }

        std::string mid_word = line.substr(0, line.find(' '));

        if (mid_word == word) {
            break;
        } else if (mid_word < word) {
            start = mid;
        } else {
            end = mid;
        }
    }

    int max_results = 100;
    index_file.seekg(start);
    while (std::getline(index_file, line) && index_file.tellg() <= end) {
        std::string line_word;
        int position;
        std::istringstream ss(line);
        ss >> line_word;

        std::cout << "Checking word in index: " << line_word << std::endl;

        if (line_word == word) {
            ss >> position;
            matches.push_back(position);

            if (matches.size() >= max_results) {
                std::cout << "Reached max result limit, exiting early.\n";
                break;
            }
        } else if (line_word > word) {
            break;
        }
    }

    return matches;
}



void show_occurrences(const std::vector<int> &positions, std::ifstream &text_file, const std::string &word, int context_size = 30) {
    std::cout << "Found " << positions.size() << " instances of the word \"" << word << "\":\n";
    std::cout << std::string(50, '-') << "\n";

    int displayed = 0;
    const int max_display = 25;
    std::string choice;

    for (auto pos : positions) {
        if (displayed >= max_display) {
            std::cout << "Shown " << max_display << " results. Show more? (y/n): ";
            std::cin >> choice;
            if (choice != "y") break;
            displayed = 0;
            std::cout << std::string(50, '-') << "\n";
        }

        text_file.seekg(std::max(0, pos - context_size));

        std::vector<char> before(context_size + 1, '\0');
        std::vector<char> after(context_size + 1, '\0');
        std::string found_word;

        text_file.read(before.data(), context_size);
        text_file >> found_word;
        text_file.read(after.data(), context_size);

        std::cout << std::setw(3) << (++displayed) << ". ... " << before.data()
                  << "***" << "\033[1;33m"<< found_word <<"\033[0m"<< "***" << after.data() << " ...\n";
        std::cout << std::string(50, '-') << "\n";
    }

    std::cout << "End of results.\n";
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <search_term>\n";
        return 1;
    }
    initialize();
    std::string search_word = argv[1];

    std::ifstream text_stream("korpus");
    std::ifstream index_stream("rawindex.txt");

    if (!text_stream || !index_stream) {
        std::cerr << "Error: Cannot open files.\n";
        return 1;
    }

    std::vector<int> index_table(27000, -1);
    create_index(index_table, index_stream);

    std::string search_lower = to_lowercase(search_word);

    std::vector<int> result_positions = find_word_positions(index_table, index_stream, search_lower);

    if (result_positions.empty()) {
        std::cout << "No matches found for the word: " << search_word << std::endl;
    } else {
        show_occurrences(result_positions, text_stream, search_word);
    }

    text_stream.close();
    index_stream.close();

    return 0;
}

/*int main() {

    initialize();
    std::string search_word;
    std::cout << "Enter the word you want to search for: ";
    std::getline(std::cin, search_word);

    // Load necessary files
    std::ifstream text_stream("korpus");
    std::ifstream index_stream("rawindex.txt");

    if (!text_stream || !index_stream) {
        std::cerr << "Error: Cannot open files.\n";
        return 1;
    }
    std::vector<int> index_table(27000, -1);
    create_index(index_table, index_stream);

    std::string search_lower = to_lowercase(search_word);

    std::vector<int> result_positions = find_word_positions(index_table, index_stream, search_lower);

    if (result_positions.empty()) {
        std::cout << "No matches found for the word: " << search_word << std::endl;
    } else {
        show_occurrences(result_positions, text_stream, search_word);
    }

    text_stream.close();
    index_stream.close();

    return 0;
}*/


#include <iostream>
#include <string>

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

int main() {
    std::string word;

    std::cout << "Enter a word : ";
    std::cin >> word;

    int hash_value = hash_value_(word);

    std::cout << "The hash value of '" << word << "' is: " << hash_value << std::endl;

    return 0;
}

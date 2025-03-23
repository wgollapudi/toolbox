#include <iostream>
#include <sstream>
#include <string>

static const std::string LOREM =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."
    "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur."
    "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

void print_chars(int n) {
    std::cout << LOREM.substr(0, std::min<int>(n, LOREM.size()));
}

void print_words(int n) {
    std::istringstream iss(LOREM);
    std::string word;
    for (int i = 0; i < n && iss >> word; i++) {
         std::cout << word << (i + 1 < n ? " ": "");
    }
}

void print_sentances(int n) {
    std::istringstream iss(LOREM);
    std::string sentance;
    int count = 0;
    while (count < n && std::getline(iss, sentance, '.')) {
        if (!sentance.empty()) {
            std::cout << sentance << '.';
            count++;
            if (count < n) std::cout << ' ';
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << LOREM;
        return 0;
    }

    if (argc != 3) {
        std::cerr << "Usage: lipsum [-w N | -s N | -c N]\n";
        return 1;
    }

    std::string flag = argv[1];
    int value = std::stoi(argv[2]);

    if (value <= 0) {
        std::cerr << "Error: N must be > 0\n";
        return 1;
    }

    if (flag == "-w") print_words(value);
    if (flag == "-s") print_sentances(value);
    if (flag == "-c") print_chars(value);
    else {
        std::cerr << "Unknown flag: " << flag << "\n";
        return 1;
    }

    return 0;
}

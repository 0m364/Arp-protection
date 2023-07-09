#include <iostream>
#include <string>
#include <fstream>
#include <regex>

bool searchForARPPoisoning(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    std::regex arpPoisoningRegex(R"(([0-9a-fA-F]{2}[:-]){5}[0-9a-fA-F]{2})");
    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, arpPoisoningRegex)) {
            std::cout << "ARP poisoning detected: " << match.str() << std::endl;
            return true;
        }
    }

    std::cout << "No ARP poisoning detected." << std::endl;
    return false;
}

int main() {
    std::string filePath = "arp_log.txt";  // Provide the path to your ARP log file
    searchForARPPoisoning(filePath);

    return 0;
}

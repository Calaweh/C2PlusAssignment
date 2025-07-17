#include <iostream>
#include "universalFunction/helperFunctions.hpp"

int main() {
    std::string text = "This is a test string with some verylongwordthatneedstobesplit and normal words";
    int max_length = 10;
    
    auto lines = splitStringBySpace(text, max_length);
    
    for (const auto& line : lines) {
        std::cout << "|" << line << "| (" << line.length() << ")\n";
    }
    
    return 0;
}
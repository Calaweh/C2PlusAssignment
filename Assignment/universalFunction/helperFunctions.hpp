#pragma once 
#include <string>
#include <vector>

std::string getPrimaryKey(const std::string &className);
std::vector<std::string> splitStringBySpace(const std::string& STR, int MAX_STRING_LENGTH_1_LINE);
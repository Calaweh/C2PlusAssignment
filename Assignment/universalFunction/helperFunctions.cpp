#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <cmath>

#include "globalFile.hpp"
#include "helperFunctions.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

std::string getPrimaryKey(const std::string &className)
{
    int primaryKeyNum = 1;
    json data;

    if (fs::exists(PRIMARY_KEY_FILE) && fs::file_size(PRIMARY_KEY_FILE) > 0)
    {

        std::ifstream file(PRIMARY_KEY_FILE);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for reading");

        try
        {
            file >> data;
            if (data.contains(className))
            {
                primaryKeyNum = data[className].get<int>();
                data[className] = primaryKeyNum + 1;
            }
            else
                data[className] = 2;
        }
        catch (const json::parse_error &e)
        {
            file.close();
            throw std::runtime_error("Failed to parse JSON data: " + std::string(e.what()));
        }
        file.close();
    }
    else
        data[className] = 2;

    std::ofstream file(PRIMARY_KEY_FILE);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file for writing");

    file << data.dump(4);
    file.close();

    std::ostringstream oss;
    oss << std::setw(MAX_PRIMARY_KEY_DIGITS) << std::setfill('0') << primaryKeyNum;
    // insert '0' in front of primary key number
    return oss.str();
}

std::vector<std::string> splitStringBySpace(const std::string &STR, const int MAX_STRING_LENGTH_1_LINE)
{
    std::vector<std::string> result;

    if (STR.empty() || MAX_STRING_LENGTH_1_LINE <= 0)
    {
        return result;
    }

    std::istringstream iss(STR);
    std::string currentLine;
    std::string word;

    while (iss >> word)
    {
        while (word.length() > MAX_STRING_LENGTH_1_LINE)
        {
            if (!currentLine.empty())
            {
                result.push_back(currentLine);
                currentLine.clear();
            }

            result.push_back(word.substr(0, MAX_STRING_LENGTH_1_LINE - 1) + '-');

            word = word.substr(MAX_STRING_LENGTH_1_LINE - 1);
        }

        if (currentLine.empty())
        {
            currentLine = word;
        }
        else
        {
            if (currentLine.length() + 1 + word.length() <= MAX_STRING_LENGTH_1_LINE)
            {
                currentLine += " " + word;
            }
            else
            {
                result.push_back(currentLine);
                currentLine = word;
            }
        }
    }

    if (!currentLine.empty())
    {
        result.push_back(currentLine);
    }

    return result;
}

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>

#include "globalFile.hpp"

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

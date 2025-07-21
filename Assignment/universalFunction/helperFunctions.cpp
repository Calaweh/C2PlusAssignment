#include <iostream>
#include "../json/single_include/nlohmann/json.hpp"
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <type_traits>

#include "globalFile.hpp"
#include "helperFunctions.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

bool IOSubManager::getIsValid() { return isValid; }
void IOSubManager::setInvalid()
{
    this->isValid = false;
    this->selection = 0;
}

int IOSubManager::getSelection() { return selection; }
void IOSubManager::setSelection(int selection)
{
    this->selection = selection;
    this->isValid = true;
}

void IOSubManager::gotError(std::string errorMessage)
{
    this->errorMessage = errorMessage;
    setInvalid();
}
void IOSubManager::setValid() { 
    this->errorMessage.clear();
    this->isValid = true;
}



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

void stoiWithLimit(IOSubManager &iOSubManager, const int &MIN_NUM, const int &MAX_NUM)
{
    stoa<int>(iOSubManager);
    if (iOSubManager.getIsValid())
    {
        int value = iOSubManager.getSelection();
        if (value < MIN_NUM || value > MAX_NUM) {
            iOSubManager.gotError("Invalid Selection. Please try again.");
        }
    }
    return;
}

template <typename T>
void stoa(IOSubManager &IOSubManager)
{
    size_t pos;
    try
    {
        if constexpr (std::is_same_v<T, int>)
        {
            int value = std::stoi(IOSubManager.input, &pos);
            if (pos != IOSubManager.input.length())
            {
                IOSubManager.gotError("Invalid numeric conversion");
                return;
            }
            else
            {
                IOSubManager.setSelection(value);
                return;
            }
        }
        // else if constexpr (std::is_same_v<T, double>)
        // {
        //     double value = std::stod(IOSubManager.input, &pos);
        //     if (pos != IOSubManager.input.length())
        //     {
        //         IOSubManager.setInvalid();
        //         return;
        //     }
        //     else
        //     {
        //         IOSubManager.setSelection(value);
        //         return;
        //     }
        // }
        IOSubManager.gotError("Unsupported type");
    }
    catch (...)
    {
        IOSubManager.gotError("Invalid numeric conversion");
    }
}
#pragma once 
#include <string>
#include <vector>
// #include <memory>

struct IOSubManager
{
    std::string input;
    std::string errorMessage;
    bool isLoop;

    void setInvalid();
    bool getIsValid();
    void setSelection(int selection);
    int getSelection();
    void gotError(std::string errorMessage);
    void setValid();
    void printErrorMessage();
    bool receiveNonEmptyInput();
    void displayErrorHolded();

    IOSubManager() : isValid(true), selection(0), isLoop(true) {}

private:
    bool isValid;
    int selection;
};

std::string getPrimaryKey(const std::string &className);
std::vector<std::string> splitStringBySpace(const std::string& STR, int MAX_STRING_LENGTH_1_LINE);
template <typename T>
void stoa(IOSubManager &);
void stoiWithLimit(IOSubManager &iOSubManager, const int &MIN_NUM, const int &MAX_NUM);

// template void stoa<int>(IOSubManager &);
// template void stoa<double>(IOSubManager &);

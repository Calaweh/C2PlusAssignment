#include "pages.hpp"
#include <iostream>

CustomerPage::CustomerPage(App::Customer& customer) 
    : customer(customer) {}

void CustomerPage::display(ScreenController &screenController)
{
    std::cout << "Hello" << std::endl;
}
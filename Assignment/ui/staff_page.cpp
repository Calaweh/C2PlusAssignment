#include "pages.hpp"
#include <iostream>

StaffPage::StaffPage(App::Staff& staff)
    : staff(staff) {} 

void StaffPage::display(ScreenController &screenController)
{
    std::cout << "Hello" << std::endl;
}
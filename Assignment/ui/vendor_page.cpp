#include "pages.hpp"
#include <iostream>

VendorPage::VendorPage(App::Vendor& vendor)
    : vendor(vendor) {}  

void VendorPage::display(ScreenController &screenController)
{
    std::cout << "Hello" << std::endl;
}
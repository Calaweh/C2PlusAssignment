#include "screen_controller.hpp"

ScreenController::ScreenController(MasterManager &masterManager, IOSubManager &iOSubManager)
    : masterManager(masterManager),
      iOSubManager(iOSubManager) {}

void ScreenController::navigateToLogin()
{
    currentPage = std::make_unique<LoginPage>("", ""); // empty password and email or ID
    currentPage->display(*this);
}

void ScreenController::navigateToVendorPage(App::Vendor &vendor)
{
    currentPage = std::make_unique<VendorPage>(vendor);
    currentPage->display(*this);
}

void ScreenController::navigateToStaffPage(App::Staff &staff)
{
    currentPage = std::make_unique<StaffPage>(staff);
    currentPage->display(*this);
}

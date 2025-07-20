#pragma once
#include "page.hpp"
#include "pages.hpp"
#include "../entityManager/master_manager.hpp"
#include <memory>

class ScreenController
{
private:
    std::unique_ptr<Page> currentPage;
    bool isRunning = true;
    MasterManager &masterManager;

public:
    ScreenController(MasterManager &masterManager);

    void navigateToLogin();
    void navigateToCustomerPage(App::Customer &customer);
    void navigateToVendorPage(App::Vendor &vendor);
    void navigateToStaffPage(App::Staff &staff);

    void exit() { isRunning = false; }

    void clearScreen();

    const MasterManager& getMasterManager() const { return masterManager; }
};

// {
//     ScreenController screenController;

//     void displayVendorOptions();
//     void displayBookingMenu();
//     void displayStallMenu();
//     void displayProductMenu();
//     void displayTransactionMenu();
//     void displayProfileMenu();
// };

// struct CustomerPage
// {
//     ScreenController screenController;

//     void displayCustomerOptions();
//     void displayDeliveryMenu();
//     void displayProductMenu();
//     void displayPaymentMenu();
//     void displayProfileMenu();
// };

// struct StaffPage
// {
//     ScreenController screenController;

//     void displayStaffOptions();
//     void displayEventMenu();
//     void displayProfileMenu();
//     void displayPromotionMenu();
//     void displayReportMenu();
// };

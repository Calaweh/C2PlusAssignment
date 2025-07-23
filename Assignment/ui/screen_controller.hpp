#pragma once
#include "page.hpp"
#include "pages.hpp"
#include "../entityManager/master_manager.hpp"
#include "../universalFunction/helperFunctions.hpp"
#include <memory>

struct ScreenController
{
private:
    std::unique_ptr<Page> currentPage;
    MasterManager &masterManager;
    IOSubManager &iOSubManager;

public:
    ScreenController(MasterManager &masterManager, IOSubManager &iOSubManager);

    void navigateToLogin();
    // void navigateToCustomerPage(App::Customer &customer);
    void navigateToVendorPage(App::Vendor &vendor);
    void navigateToStaffPage(App::Staff &staff);

    void exit();

    void clearScreen();

    MasterManager& getMasterManager() const { return masterManager; }
    IOSubManager& getIOSubManager() const { return iOSubManager; }
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

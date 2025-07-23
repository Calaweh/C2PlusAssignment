#pragma once
#include <string>
#include "page.hpp"
#include "../universalFunction/helperFunctions.hpp"
#include "../entityManager/master_manager.hpp"

namespace App {
    class Customer;
    class Vendor;
    class Staff;
}

class LoginPage : public Page {

public:
    explicit LoginPage() = default;
    explicit LoginPage(std::string emailOrId, std::string password);
    void display(ScreenController& screenController) override; 
    void accountPrompt(IOSubManager &iOSubManager);
    void switchPage(ScreenController& screenController, IOSubManager &iOSubManager, bool &isBack);
    void SignUpPage(IOSubManager &iOSubManager, MasterManager &masterManager);
    void ForgotPasswordPage();
    
private:
    std::string emailOrId;
    std::string password;
    int accountType; //1 for Vendor, 2 for Staff
};



class CustomerPage : public Page {
public:
    explicit CustomerPage(App::Customer& customer);
    void display(ScreenController& screenController) override;  
    
private:
    App::Customer& customer;  
};



class VendorPage : public Page {
public:
    explicit VendorPage(App::Vendor& vendor);
    void display(ScreenController& screenController) override;  
    
private:
    App::Vendor& vendor;  
};



class StaffPage : public Page {
public:
    explicit StaffPage(App::Staff& staff);
    void display(ScreenController& screenController) override; 
    
private:
    App::Staff& staff; 
};
#pragma once
#include <string>
#include "page.hpp"
#include "../universalFunction/helperFunctions.hpp"

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
    void switchPage(ScreenController& screenController, IOSubManager &IOSubManager, bool &isBack);
    void SignUpPage();
    void ForgotPasswordPage();
    
private:
    std::string emailOrId;
    std::string password;
    int accountType; //1 for Customer, 2 for Vendor, 3 for Staff
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
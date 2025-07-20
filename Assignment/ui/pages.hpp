#pragma once
#include "page.hpp"
#include <string>

// Forward declarations
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
    void accountPrompt(std::string &errorMessage);
    
private:
    std::string emailOrId;
    std::string password;
};

class CustomerPage : public Page {
public:
    explicit CustomerPage(App::Customer& customer);
    void display(ScreenController& screenController) override;  // Proper override
    
private:
    App::Customer& customer;  // Using reference as per your design
};

class VendorPage : public Page {
public:
    explicit VendorPage(App::Vendor& vendor);
    void display(ScreenController& screenController) override;  // Proper override
    
private:
    App::Vendor& vendor;  // Using reference as per your design
};

class StaffPage : public Page {
public:
    explicit StaffPage(App::Staff& staff);
    void display(ScreenController& screenController) override;  // Proper override
    
private:
    App::Staff& staff;  // Using reference as per your design
};
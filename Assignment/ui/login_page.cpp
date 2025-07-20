#include "pages.hpp"
#include "helperFunctions.hpp"
#include "screen_controller.hpp"
#include <format>
#include "json_database.hpp"
#include <iostream>

using IdentityReturn = std::variant<
    std::monostate,
    std::shared_ptr<App::Customer>,
    std::shared_ptr<App::Vendor>,
    std::shared_ptr<App::Staff>>;

LoginPage::LoginPage(std::string emailOrId, std::string password)
    : emailOrId(std::move(emailOrId)),
      password(std::move(password)) {}

void LoginPage::display(ScreenController &screenController)
{
    bool isError = false;
    std::string errorMessage;
    std::string contentDisplayed = std::string("1) Customer\n2) Vendor\n3) Staff\n0) Exit");
    std::string input;
    int type = 0;

    while (true)
    {

        std::cout << contentDisplayed << std::endl;

        if (isError)
        {
            std::cout << errorMessage << std::endl;
            isError = !isError;
        }

        if (!getline(std::cin, input))
        {
        }

        type = stoa<int>(input);
        if (type == -1)
        {
            isError = true;
            errorMessage = "Invalid input. Please enter a number.";
            continue;
        }
        break;
    }

    while (true)
    {
        accountPrompt(errorMessage);

        auto &identityManager = screenController.getMasterManager().getIdentityManager();

        auto identity = identityManager.accountVerification(this->emailOrId, this->password, type);

        switch (type)
        {
        case 1:
            if (auto customer = std::get_if<std::shared_ptr<App::Customer>>(&identity))
            {
                if (!*customer) 
                screenController.navigateToCustomerPage(**customer);
            }
            return;
        case 2:
            if (auto vendor = std::get_if<std::shared_ptr<App::Vendor>>(&identity))
            {
                if (!*vendor)
                    break;
                screenController.navigateToVendorPage(**vendor);
            }
            return;
        case 3:
            if (auto staff = std::get_if<std::shared_ptr<App::Staff>>(&identity))
            {
                if (!*staff) 
                    break;
                screenController.navigateToStaffPage(**staff);
            }
            return;
        default:
            errorMessage = "Account doesn't exist or invalid password. Please try again.";
            continue;
        }
        errorMessage = "Account doesn't exist or invalid password. Please try again.";
        continue;
    }
}

void LoginPage::accountPrompt(std::string &errorMessage)
{
    std::string emailOrId, pwd;
    std::string contentDisplayed = std::format(
        " *Email or ID: {0}\n *Password: {1}\n0) Exit", this->emailOrId, this->password);

    std::cout << contentDisplayed << std::endl;

    if (!errorMessage.empty())
    {
        std::cout << errorMessage << std::endl;
        errorMessage.clear();
    }

    std::cout << "Please enter your Email or ID: ";
    getline(std::cin, this->emailOrId);

    std::cout << "Please enter your Password: ";
    getline(std::cin, this->password);
}
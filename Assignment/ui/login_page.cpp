#include <format>
#include <iostream>
#include "pages.hpp"
#include "helperFunctions.hpp"
#include "screen_controller.hpp"
#include "json_database.hpp"

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
    std::string contentDisplayed = std::string("1) Vendor\n2) Staff\n0) Exit");
    std::string input;
    bool isBack = false;

    do
    {
        isBack = false;
        while (true)
        {

            std::cout << contentDisplayed << std::endl;

            if (!screenController.getIOSubManager().getIsValid())
            {
                std::cout << screenController.getIOSubManager().errorMessage << std::endl;
                screenController.getIOSubManager().setValid();
            }

            if (!getline(std::cin, screenController.getIOSubManager().input))
            {
            }

            stoa<int>(screenController.getIOSubManager());

            stoiWithLimit(screenController.getIOSubManager(), 0, 2);

            if (!screenController.getIOSubManager().getIsValid()) {
                continue;
            }
            this->accountType = screenController.getIOSubManager().getSelection();
            break;
        }

        switchPage(screenController, screenController.getIOSubManager(), isBack);

    } while (isBack);
}

void LoginPage::switchPage(ScreenController &screenController, IOSubManager &iOSubManager, bool &isBack)
{
    while (true)
    {
        accountPrompt(iOSubManager);

        // std::cout << "2" << std::endl; /////////////////////

        if (iOSubManager.getSelection() == 0 && iOSubManager.getIsValid()){
            isBack = true;
            return; 
        }

        switch (iOSubManager.getSelection())
        {
        case -1:
        // std::cout << "3" << std::endl; /////////////////////
            SignUpPage(iOSubManager, screenController.getMasterManager());
            continue;
        case -2:
            ForgotPasswordPage();
            continue;
        }

        auto &identityManager = screenController.getMasterManager().getIdentityManager();

        auto identity = identityManager.accountVerification(this->emailOrId, this->password, this->accountType);

        switch (this->accountType)
        {
        // case 1:
        //     if (auto customer = std::get_if<std::shared_ptr<App::Customer>>(&identity))
        //     {
        //         if (!*customer)
        //             break;
        //         screenController.navigateToCustomerPage(**customer);
        //     }
        //     return;
        case 1:
            if (auto vendor = std::get_if<std::shared_ptr<App::Vendor>>(&identity))
            {
                if (!*vendor)
                    break;
                screenController.navigateToVendorPage(**vendor);
            }
            return;
        case 2:
            if (auto staff = std::get_if<std::shared_ptr<App::Staff>>(&identity))
            {
                if (!*staff)
                    break;
                screenController.navigateToStaffPage(**staff);
            }
            return;
        default:
            iOSubManager.gotError("Account doesn't exist or invalid password. Please try again.");
            isBack = true;
            continue;
        }
        iOSubManager.gotError("Account doesn't exist or invalid password. Please try again.");
        isBack = true;
        continue;
    }
}

void LoginPage::accountPrompt(IOSubManager &iOSubManager)
{
    std::string input;

    std::string contentDisplayed = std::format(
        " *Email or ID: {0}\n *Password: {1}\n 0) Back \n-1) Sign Up \n-2) Forgot Password", this->emailOrId, this->password);

    std::cout << contentDisplayed << std::endl;

    if (!iOSubManager.errorMessage.empty())
    {
        std::cout << iOSubManager.errorMessage << std::endl;
        iOSubManager.setValid();
    }

    std::cout << "Please enter your Email or ID: ";
    getline(std::cin, iOSubManager.input);

    stoiWithLimit(iOSubManager, -2, 0);
    if (iOSubManager.getIsValid()) 
        return;

    this->emailOrId = input;

    std::cout << "Please enter your Password: ";
    getline(std::cin, iOSubManager.input);

    stoiWithLimit(iOSubManager, -2, 0);
    if (iOSubManager.getIsValid())
        return;

    this->password = input;

    return;
}

void LoginPage::SignUpPage(IOSubManager &iOSubManager, MasterManager &masterManager)
{
    system("cls");
    // std::cout << "1" << std::endl; /////////////////////
    masterManager.getIdentityManager().createAccount(iOSubManager, this->accountType);
    system("cls");
    return;
}

void LoginPage::ForgotPasswordPage()
{
}

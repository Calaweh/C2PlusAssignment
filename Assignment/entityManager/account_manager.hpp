#pragma once
#include "../entity/entities.hpp"
#include "../universalFunction/helperFunctions.hpp"
#include "entities_manager.hpp"
#include <iostream>
#include <variant>

template <typename T>
std::shared_ptr<T> accountVerification(const std::unordered_set<std::shared_ptr<T>> &ENTITIES,
                                       const std::string &EMAIL_OR_ID,
                                       const std::string &PWD)
{
    // std::cout << "4" << std::endl; /////////////////////
    for (const auto &entity : ENTITIES)
    {
        if (!entity)
            continue;

        if ((entity->email == EMAIL_OR_ID || entity->Id == EMAIL_OR_ID) && entity->pwd == PWD)
            return entity;
    }

    return nullptr;
}

// struct CustomerManager : public EntitiesManager<App::Customer>
// {

//     CustomerManager() : EntitiesManager<App::Customer>(CUSTOMERS) {}

//     std::shared_ptr<App::Customer> accountVerification(const std::string &EMAIL_OR_ID,
//                                                        const std::string &PWD) const
//     {
//         // std::cout << "5" << std::endl; /////////////////////
//         return ::accountVerification(this->entities, EMAIL_OR_ID, PWD);
//     }
// };

struct VendorManager : public EntitiesManager<App::Vendor>
{
    VendorManager() : EntitiesManager<App::Vendor>(VENDORS) {}

    std::shared_ptr<App::Vendor> accountVerification(const std::string &EMAIL_OR_ID,
                                                     const std::string &PWD) const
    {
        return ::accountVerification(this->entities, EMAIL_OR_ID, PWD);
    }
};

struct StaffManager : public EntitiesManager<App::Staff>
{
    StaffManager() : EntitiesManager<App::Staff>(STAFFS) {}

    std::shared_ptr<App::Staff> accountVerification(const std::string &EMAIL_OR_ID,
                                                    const std::string &PWD) const
    {
        return ::accountVerification(this->entities, EMAIL_OR_ID, PWD);
    }
};

// struct AddressManager : public EntitiesManager<App::Address>
// {
//     AddressManager() : EntitiesManager<App::Address>(ADDRESSES) {}

//     std::vector<App::Address> &createAddresses(IOSubManager &iOSubManager) {
//         while (true) {
//             std::unique_ptr<App::Address> address = std::make_unique<App::Address>();

//         }
//     }
// };

struct IdentityManager : public EntitiesManager<App::Identity>
{
    using IdentityReturn = std::variant<
        std::monostate,
        // std::shared_ptr<App::Customer>,
        std::shared_ptr<App::Vendor>,
        std::shared_ptr<App::Staff>>;

    // CustomerManager customerManager;
    VendorManager vendorManager;
    StaffManager staffManager;

    IdentityManager(VendorManager &vendorManager, StaffManager &staffManager) : EntitiesManager<App::Identity>(IDENTITIES), vendorManager(vendorManager), staffManager(staffManager) {}

    IdentityReturn accountVerification(const std::string &EMAIL_OR_ID, const std::string &PWD, const int &TYPE) const
    {
        switch (TYPE)
        {
        // case 1:
        //     return customerManager.accountVerification(EMAIL_OR_ID, PWD);
        //     break;
        case 1:
            return vendorManager.accountVerification(EMAIL_OR_ID, PWD);
            break;
        case 2:
            return staffManager.accountVerification(EMAIL_OR_ID, PWD);
            break;
        default:
            return std::monostate{};
        }
    }

    bool createAccount(IOSubManager &iOSubManager, const int &TYPE)
    {
        int iteratorForDataFields = 0;
        std::string dataFieldNames[4] =
            {
                "Account Name",
                "Account Email",
                "Phone Number",
                "Password"};

        std::vector<std::string> dataFieldValues = {"","","",""};
        App::Identity account;
        bool isFill = false;

        do
        {
            system("cls");
            for (int i = 0; i < 4; i++)
            {
                std::cout << std::format("{:>2}) {}: {}", i + 1, dataFieldNames[i], dataFieldValues[i]) << std::endl;
            }
            std::cout << SEPARATOR_2LINE << std::endl;
            iOSubManager.displayErrorHolded();

            std::cout << " 0) Exit" << std::endl;

            if (isFill)
            {
                std::cout << std::format(" s) save\n *or Enter Field Number to Modify") << std::endl;
            }
            else
            {
                std::cout << std::format("Please enter {}: ", dataFieldNames[iteratorForDataFields]);
            }

            if (!iOSubManager.receiveNonEmptyInput())
                continue;

            stoiWithLimit(iOSubManager, 0, 4);
            if (iOSubManager.getIsValid() )
            {
                switch (iOSubManager.getSelection())
                {
                case 0:
                    return false;
                default:
                    if (isFill)
                    {
                        iteratorForDataFields = iOSubManager.getSelection();
                        std::cout << std::format("Please enter {}: ", dataFieldNames[iteratorForDataFields]);
                        if (!iOSubManager.receiveNonEmptyInput())
                            continue;
                    }
                }
            }
            else if (isFill && iOSubManager.input == "s")
            {
                switch (TYPE)
                {
                case 1:
                {
                    auto vendor = std::make_shared<App::Vendor>();
                    vendor->Id = getPrimaryKey(VENDOR);
                    vendor->name = dataFieldValues[0];
                    vendor->setEmail(dataFieldValues[1]);
                    vendor->setPhoneNumber(dataFieldValues[2]);
                    vendor->setPwd(dataFieldValues[3]);
                    vendorManager.createOrUpdateEntity(*vendor);
                    return true;
                }
                default:
                {
                    auto staff = std::make_shared<App::Staff>();
                    staff->Id = getPrimaryKey(STAFF);
                    staff->name = dataFieldValues[0];
                    staff->setEmail(dataFieldValues[1]);
                    staff->setPhoneNumber(dataFieldValues[2]);
                    staff->setPwd(dataFieldValues[3]);
                    staffManager.createOrUpdateEntity(*staff);
                    return true;
                }
                }
            }
            iOSubManager.setValid();

            try
            {
                switch (iteratorForDataFields)
                {
                case 0:
                    account.name = dataFieldValues[iteratorForDataFields];
                    break;
                case 1:
                    account.setEmail(dataFieldValues[iteratorForDataFields]);
                    break;
                case 2:
                    account.setPhoneNumber(dataFieldValues[iteratorForDataFields]);
                    break;
                case 3:
                    account.setPwd(dataFieldValues[iteratorForDataFields]);
                    break;
                default:
                    break;
                }
                dataFieldValues[iteratorForDataFields] = iOSubManager.input;
                iteratorForDataFields++;

                if (iteratorForDataFields == 4) {
                    isFill = true;
                } 
            }
            catch (const std::exception &e)
            {
                iOSubManager.gotError(e.what());
                iteratorForDataFields--;
                continue;
            }

        } while (iOSubManager.isLoop);
    }
};
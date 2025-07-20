#pragma once
#include "../entity/entities.hpp"
#include "entities_manager.hpp"
#include <iostream>
#include <variant>

template <typename T>
std::shared_ptr<T> accountVerification(const std::unordered_set<std::shared_ptr<T>> &ENTITIES,
                                       const std::string &EMAIL_OR_ID,
                                       const std::string &PWD)
{
    std::cout << "4" << std::endl; /////////////////////
    for (const auto &entity : ENTITIES)
    {
        if (!entity)
            continue;

        if ((entity->email == EMAIL_OR_ID || entity->Id == EMAIL_OR_ID) && entity->pwd == PWD)
            return entity;
    }

    return nullptr;
}

struct CustomerManager : public EntitiesManager<App::Customer>
{

    CustomerManager() : EntitiesManager<App::Customer>(CUSTOMERS) {}

    std::shared_ptr<App::Customer> accountVerification(const std::string &EMAIL_OR_ID,
                                                       const std::string &PWD) const
    {
        std::cout << "5" << std::endl; /////////////////////
        return ::accountVerification(this->entities, EMAIL_OR_ID, PWD);
    }
};

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

struct AddressManager : public EntitiesManager<App::Address>
{
    AddressManager() : EntitiesManager<App::Address>(ADDRESSES) {}
};

struct IdentityManager : public EntitiesManager<App::Identity>
{
    using IdentityReturn = std::variant<
        std::monostate,
        std::shared_ptr<App::Customer>,
        std::shared_ptr<App::Vendor>,
        std::shared_ptr<App::Staff>>;

    CustomerManager customerManager;
    VendorManager vendorManager;
    StaffManager staffManager;

    IdentityManager(CustomerManager &customerManager, VendorManager &vendorManager, StaffManager &staffManager) : EntitiesManager<App::Identity>(IDENTITIES), customerManager(customerManager), vendorManager(vendorManager), staffManager(staffManager) {}

    IdentityReturn accountVerification(const std::string &EMAIL_OR_ID, const std::string &PWD, int type) const
    {
        switch (type)
        {
        case 1:
            std::cout << "1" << std::endl; /////////////////////
            return customerManager.accountVerification(EMAIL_OR_ID, PWD);
            break;
        case 2:
            return vendorManager.accountVerification(EMAIL_OR_ID, PWD);
            break;
        case 3:
            return staffManager.accountVerification(EMAIL_OR_ID, PWD);
            break;
        default:
            return std::monostate{};
        }
    }
};
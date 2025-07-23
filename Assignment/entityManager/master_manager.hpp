#pragma once
#include "account_manager.hpp"
#include "event_manager.hpp"
#include "logistic_manager.hpp"

struct MasterManager {
private:
    // std::unique_ptr<CustomerManager> customerManager;
    std::unique_ptr<VendorManager> vendorManager;
    std::unique_ptr<StaffManager> staffManager;
    // std::unique_ptr<AddressManager> addressManager;
    std::unique_ptr<IdentityManager> identityManager;

public:
    MasterManager()
        : 
        // customerManager(std::make_unique<CustomerManager>()),
          vendorManager(std::make_unique<VendorManager>()),
          staffManager(std::make_unique<StaffManager>()),
        //   addressManager(std::make_unique<AddressManager>()),
          identityManager(std::make_unique<IdentityManager>(*vendorManager, *staffManager)) {}

    // CustomerManager& getCustomerManager() { return *customerManager; }
    VendorManager& getVendorManager() { return *vendorManager; }
    StaffManager& getStaffManager() { return *staffManager; }
    // AddressManager& getAddressManager() { return *addressManager; }
    IdentityManager& getIdentityManager() { return *identityManager; }

    // const CustomerManager& getCustomerManager() const { return *customerManager; }
    const VendorManager& getVendorManager() const { return *vendorManager; }
    const StaffManager& getStaffManager() const { return *staffManager; }
    // const AddressManager& getAddressManager() const { return *addressManager; }
    const IdentityManager& getIdentityManager() const { return *identityManager; }
};